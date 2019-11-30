#include <algorithm>
#include <chrono>

#include "camera/environment.h"
#include "camera/pinhole.h"
#include "core/transform.h"
#include "integrator/pt.h"
#include "light/light.h"
#include "parallel/parallel.h"
#include "postprocess/tone_mapping.h"
#include "renderer/renderer.h"
#include "renderer/scene_loader.h"
#include "sampler/random.h"
#include "sky/hosek_sky.h"
#include "sky/ibl_sky.h"
#include "sky/uniform_sky.h"

namespace Prl2 {

void Renderer::loadConfig(const RenderConfig& _config) {
  // RenderConfigのセット
  config = _config;

  // Layerの初期化
  layer.resize(_config.width, _config.height);

  // シーンファイルの読み込み
  if (!config.scene_file.empty()) {
    SceneLoader sceneLoader;
    sceneLoader.loadSceneFromToml(config.scene_file, scene);
  }

  // Skyの設定
  commitSky();

  // Filmの設定
  const auto film = std::make_shared<Film>(
      config.width, config.height, config.width_length, config.height_length);

  // Cameraの設定
  commitCamera();

  // Samplerの設定
  if (!config.sampler_type.empty()) {
    if (config.sampler_type == "random") {
      sampler = std::make_shared<RandomSampler>();
    } else {
      sampler = std::make_shared<RandomSampler>();
    }
  } else {
    sampler = std::make_shared<RandomSampler>();
  }

  // Integratorの設定
  if (!config.integrator_type.empty()) {
    if (config.integrator_type == "PT") {
      integrator = std::make_shared<PT>();
    } else {
      std::cerr << "invalid integrator type" << std::endl;
      std::exit(EXIT_FAILURE);
    }
  } else {
    integrator = std::make_shared<PT>();
  }
}

void Renderer::render(const std::atomic<bool>& cancel) {
  // Progressを初期化
  num_rendered_pixels = 0;

  // レイヤーを初期化
  layer.clear();

  // フィルムをクリア
  scene.camera->film->clear();

  // 時間計測
  const auto start_time = std::chrono::system_clock::now();

  // それぞれの画素で同じ処理を行う
  if (!config.render_realtime) {
    parallelFor2D(
        [&](int i, int j) {
          // Samplerの初期化
          std::unique_ptr<Sampler> pixel_sampler =
              sampler->clone(i + config.width * j);

          //サンプリングを繰り返す
          for (int k = 0; k < config.samples; ++k) {
            if (cancel) {
              break;
            }

            // 波長のサンプリング
            const Real lambda =
                SPD::LAMBDA_MIN +
                pixel_sampler->getNext() * (SPD::LAMBDA_MAX - SPD::LAMBDA_MIN);
            constexpr Real lambda_pdf = 1 / (SPD::LAMBDA_MAX - SPD::LAMBDA_MIN);

            // サンプリングされた波長をセット
            Ray ray;
            ray.lambda = lambda;

            //フィルム面のUV座標
            const Real u =
                (2.0f * (i + pixel_sampler->getNext()) - config.width) /
                config.width;
            const Real v =
                (2.0f * (j + pixel_sampler->getNext()) - config.height) /
                config.height;

            //カメラからレイを生成
            if (scene.camera->generateRay(u, v, ray)) {
              // Primary Rayで計算できるものを計算しておく
              IntersectInfo info;
              if (scene.intersector->intersect(ray, info)) {
                // Normal LayerにsRGBを加算
                layer.normal_sRGB[3 * i + 3 * config.width * j + 0] +=
                    0.5f * (info.hitNormal.x() + 1.0f);
                layer.normal_sRGB[3 * i + 3 * config.width * j + 1] +=
                    0.5f * (info.hitNormal.y() + 1.0f);
                layer.normal_sRGB[3 * i + 3 * config.width * j + 2] +=
                    0.5f * (info.hitNormal.z() + 1.0f);

                // Depth LayerにsRGBを加算
                layer.depth_sRGB[3 * i + 3 * config.width * j + 0] += info.t;
                layer.depth_sRGB[3 * i + 3 * config.width * j + 1] += info.t;
                layer.depth_sRGB[3 * i + 3 * config.width * j + 2] += info.t;

                // Position LayerにsRGBを加算
                layer.position_sRGB[3 * i + 3 * config.width * j + 0] +=
                    info.hitPos.x();
                layer.position_sRGB[3 * i + 3 * config.width * j + 1] +=
                    info.hitPos.y();
                layer.position_sRGB[3 * i + 3 * config.width * j + 2] +=
                    info.hitPos.z();

                // Sample LayerにsRGBを格納
                const auto material = info.hitPrimitive->material;
                const Vec3 wo = -ray.direction;
                const Vec3 wo_local = worldToMaterial(wo, info);
                const SurfaceInteraction interaction(wo_local, ray.lambda);
                Vec3 wi_local;
                Real pdf;
                material->sampleDirection(interaction, *pixel_sampler, wi_local,
                                          pdf);
                const Vec3 wi = materialToWorld(wi_local, info);
                layer.sample_sRGB[3 * i + 3 * config.width * j + 0] +=
                    0.5f * (wi.x() + 1.0f);
                layer.sample_sRGB[3 * i + 3 * config.width * j + 1] +=
                    0.5f * (wi.y() + 1.0f);
                layer.sample_sRGB[3 * i + 3 * config.width * j + 2] +=
                    0.5f * (wi.z() + 1.0f);
              }

              // 分光放射束の計算
              const Real phi =
                  integrator->integrate(ray, scene, *pixel_sampler) /
                  lambda_pdf;

              // フィルムに分光放射束を加算
              if (!std::isnan(phi)) {
                scene.camera->film->addPixel(i, j, lambda, phi);
              } else {
                fprintf(stderr, "nan detected at (%d, %d)\n", i, j);
              }
            }
          }

          // Render LayerにsRGBを加算
          const RGB rgb =
              (scene.camera->film->getPixel(i, j) / config.samples).toRGB();
          layer.render_sRGB[3 * i + 3 * config.width * j + 0] = rgb.x();
          layer.render_sRGB[3 * i + 3 * config.width * j + 1] = rgb.y();
          layer.render_sRGB[3 * i + 3 * config.width * j + 2] = rgb.z();

          // 他のレイヤーの寄与をサンプル数で割る
          layer.normal_sRGB[3 * i + 3 * config.width * j + 0] /= config.samples;
          layer.normal_sRGB[3 * i + 3 * config.width * j + 1] /= config.samples;
          layer.normal_sRGB[3 * i + 3 * config.width * j + 2] /= config.samples;

          layer.depth_sRGB[3 * i + 3 * config.width * j + 0] /= config.samples;
          layer.depth_sRGB[3 * i + 3 * config.width * j + 1] /= config.samples;
          layer.depth_sRGB[3 * i + 3 * config.width * j + 2] /= config.samples;

          layer.position_sRGB[3 * i + 3 * config.width * j + 0] /=
              config.samples;
          layer.position_sRGB[3 * i + 3 * config.width * j + 1] /=
              config.samples;
          layer.position_sRGB[3 * i + 3 * config.width * j + 2] /=
              config.samples;

          layer.sample_sRGB[3 * i + 3 * config.width * j + 0] /= config.samples;
          layer.sample_sRGB[3 * i + 3 * config.width * j + 1] /= config.samples;
          layer.sample_sRGB[3 * i + 3 * config.width * j + 2] /= config.samples;

          // Progressを加算
          num_rendered_pixels += 1;
        },
        config.render_tiles_x, config.render_tiles_y, config.width,
        config.height);
  } else {
    for (int k = 0; k < config.samples; ++k) {
      parallelFor2D(
          [&](int i, int j) {
            if (cancel) {
              return;
            }

            // Samplerの初期化
            std::unique_ptr<Sampler> pixel_sampler = sampler->clone(
                k + config.samples * i + config.samples * config.width * j);

            // 波長のサンプリング
            const Real lambda =
                SPD::LAMBDA_MIN +
                pixel_sampler->getNext() * (SPD::LAMBDA_MAX - SPD::LAMBDA_MIN);
            constexpr Real lambda_pdf = 1 / (SPD::LAMBDA_MAX - SPD::LAMBDA_MIN);

            // サンプリングされた波長をセット
            Ray ray;
            ray.lambda = lambda;

            //フィルム面のUV座標
            const Real u =
                (2.0f * (i + pixel_sampler->getNext()) - config.width) /
                config.width;
            const Real v =
                (2.0f * (j + pixel_sampler->getNext()) - config.height) /
                config.height;

            //カメラからレイを生成
            if (scene.camera->generateRay(u, v, ray)) {
              // Primary Rayで計算できるものを計算しておく
              IntersectInfo info;
              if (scene.intersector->intersect(ray, info)) {
                // Normal LayerにsRGBを加算
                layer.normal_sRGB[3 * i + 3 * config.width * j + 0] =
                    (k * layer.normal_sRGB[3 * i + 3 * config.width * j + 0] +
                     0.5f * (info.hitNormal.x() + 1.0f)) /
                    (k + 1);
                layer.normal_sRGB[3 * i + 3 * config.width * j + 1] =
                    (k * layer.normal_sRGB[3 * i + 3 * config.width * j + 1] +
                     0.5f * (info.hitNormal.y() + 1.0f)) /
                    (k + 1);
                layer.normal_sRGB[3 * i + 3 * config.width * j + 2] =
                    (k * layer.normal_sRGB[3 * i + 3 * config.width * j + 2] +
                     0.5f * (info.hitNormal.z() + 1.0f)) /
                    (k + 1);

                // Depth LayerにsRGBを加算
                layer.depth_sRGB[3 * i + 3 * config.width * j + 0] =
                    (k * layer.depth_sRGB[3 * i + 3 * config.width * j + 0] +
                     info.t) /
                    (k + 1);
                layer.depth_sRGB[3 * i + 3 * config.width * j + 1] =
                    (k * layer.depth_sRGB[3 * i + 3 * config.width * j + 1] +
                     info.t) /
                    (k + 1);
                layer.depth_sRGB[3 * i + 3 * config.width * j + 2] =
                    (k * layer.depth_sRGB[3 * i + 3 * config.width * j + 2] +
                     info.t) /
                    (k + 1);

                // Position LayerにsRGBを加算
                layer.position_sRGB[3 * i + 3 * config.width * j + 0] =
                    (k * layer.position_sRGB[3 * i + 3 * config.width * j + 0] +
                     info.hitPos.x()) /
                    (k + 1);
                layer.position_sRGB[3 * i + 3 * config.width * j + 1] =
                    (k * layer.position_sRGB[3 * i + 3 * config.width * j + 1] +
                     info.hitPos.y()) /
                    (k + 1);
                layer.position_sRGB[3 * i + 3 * config.width * j + 2] =
                    (k * layer.position_sRGB[3 * i + 3 * config.width * j + 2] +
                     info.hitPos.z()) /
                    (k + 1);

                // Sample LayerにsRGBを格納
                const auto material = info.hitPrimitive->material;
                const Vec3 wo = -ray.direction;
                const Vec3 wo_local = worldToMaterial(wo, info);
                const SurfaceInteraction interaction(wo_local, ray.lambda);
                Vec3 wi_local;
                Real pdf;
                material->sampleDirection(interaction, *pixel_sampler, wi_local,
                                          pdf);
                const Vec3 wi = materialToWorld(wi_local, info);
                layer.sample_sRGB[3 * i + 3 * config.width * j + 0] =
                    (k * layer.sample_sRGB[3 * i + 3 * config.width * j + 0] +
                     0.5f * (wi.x() + 1.0f)) /
                    (k + 1);
                layer.sample_sRGB[3 * i + 3 * config.width * j + 1] =
                    (k * layer.sample_sRGB[3 * i + 3 * config.width * j + 1] +
                     0.5f * (wi.y() + 1.0f)) /
                    (k + 1);
                layer.sample_sRGB[3 * i + 3 * config.width * j + 2] =
                    (k * layer.sample_sRGB[3 * i + 3 * config.width * j + 2] +
                     0.5f * (wi.z() + 1.0f)) /
                    (k + 1);
              }

              // 分光放射束の計算
              const Real phi =
                  integrator->integrate(ray, scene, *pixel_sampler) /
                  lambda_pdf;

              // フィルムに分光放射束を加算
              if (!std::isnan(phi)) {
                scene.camera->film->addPixel(i, j, lambda, phi);
              } else {
                fprintf(stderr, "nan detected at (%d, %d)\n", i, j);
              }
            }

            // Render LayerにsRGBを加算
            const RGB rgb = (scene.camera->film->getPixel(i, j) / k).toRGB();
            layer.render_sRGB[3 * i + 3 * config.width * j + 0] = rgb.x();
            layer.render_sRGB[3 * i + 3 * config.width * j + 1] = rgb.y();
            layer.render_sRGB[3 * i + 3 * config.width * j + 2] = rgb.z();

            // Progressを加算
            num_rendered_pixels += 1;
          },
          16, 16, config.width, config.height);
    }
  }

  const auto finish_time = std::chrono::system_clock::now();

  // レンダリングに要した時間をセット
  rendering_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                       finish_time - start_time)
                       .count();
}

Real Renderer::getRenderProgress() const {
  return static_cast<Real>(num_rendered_pixels) /
         (config.width * config.height);
}

int Renderer::getRenderingTime() const { return rendering_time; }

void Renderer::commitCamera() {
  const auto film = std::make_shared<Film>(
      config.width, config.height, config.width_length, config.height_length);
  const auto transform = std::make_shared<Transform>(
      lookAt(config.camera_position, config.camera_lookat));

  if (config.camera_type == CameraType::Pinhole) {
    scene.setCamera(std::make_shared<PinholeCamera>(film, transform,
                                                    config.camera_pinhole_fov));
  } else if (config.camera_type == CameraType::Environment) {
    scene.setCamera(std::make_shared<EnvironmentCamera>(film, transform));
  } else {
    std::cerr << "Invalid camera type" << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

void Renderer::getImageSize(int& sx, int& sy) const {
  sx = config.width;
  sy = config.height;
}

void Renderer::setImageSize(unsigned int width, unsigned int height) {
  config.width = width;
  config.height = height;
  layer.resize(width, height);
  scene.camera->film->resize(width, height);
}

unsigned int Renderer::getSamples() const { return config.samples; }

void Renderer::setSamples(unsigned int samples) { config.samples = samples; }

void Renderer::getRenderTiles(int& x, int& y) const {
  x = config.render_tiles_y;
  y = config.render_tiles_x;
}

void Renderer::setRenderTiles(int x, int y) {
  config.render_tiles_x = x;
  config.render_tiles_y = y;
}

LayerType Renderer::getOutputLayer() const { return config.layer_type; }

void Renderer::setOutputLayer(const LayerType& _layer_type) {
  config.layer_type = _layer_type;
}

ImageType Renderer::getImageType() const { return config.image_type; }

void Renderer::setImageType(const ImageType& _image_type) {
  config.image_type = _image_type;
}

Real Renderer::getExposure() const { return config.exposure; }

void Renderer::setExposure(const Real& exposure) { config.exposure = exposure; }

Real Renderer::getGamma() const { return config.gamma; }

void Renderer::setGamma(const Real& gamma) { config.gamma = gamma; }

ToneMappingType Renderer::getToneMappingType() const {
  return config.tone_mapping_type;
}

void Renderer::setToneMappingType(const ToneMappingType& _tone_mapping_type) {
  config.tone_mapping_type = _tone_mapping_type;
}

Real Renderer::getMappingFactor() const { return config.mapping_factor; }

void Renderer::setMappingFactor(const Real& _mapping_factor) {
  config.mapping_factor = _mapping_factor;
}

void Renderer::getFilmLength(Real& lx, Real& ly) const {
  lx = config.width_length;
  ly = config.height_length;
}

void Renderer::setFilmLength(const Real& width_length,
                             const Real& height_length) {
  config.width_length = width_length;
  config.height_length = height_length;
  scene.camera->film->resizeLength(width_length, height_length);
}

void Renderer::getCameraLookAt(Vec3& pos, Vec3& lookat) const {
  pos = config.camera_position;
  lookat = config.camera_lookat;
}

void Renderer::setCameraLookAt(const Vec3& pos, const Vec3& lookat) {
  config.camera_position = pos;
  config.camera_lookat = lookat;
  scene.camera->setLookAt(pos, lookat);
}

SkyType Renderer::getSkyType() const { return config.sky_type; }

void Renderer::setSkyType(const SkyType& sky_type) {
  config.sky_type = sky_type;
}

void Renderer::commitSky() {
  if (config.sky_type == SkyType::Uniform) {
    scene.setSky(
        std::make_shared<UniformSky>(RGB2Spectrum(config.uniform_sky_color)));
  } else if (config.sky_type == SkyType::Hosek) {
    scene.setSky(std::make_shared<HosekSky>(
        config.hosek_sky_sun_direciton, config.hosek_sky_turbidity,
        RGB2Spectrum(config.hosek_sky_albedo)));
  } else if (config.sky_type == SkyType::IBL) {
    scene.setSky(std::make_shared<IBLSky>(config.ibl_sky_filename));
  } else {
    std::cerr << "Invalid sky type" << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

Vec3 Renderer::getUniformSkyColor() const { return config.uniform_sky_color; }

void Renderer::setUniformSkyColor(const Vec3& color) {
  config.uniform_sky_color = color;
}

Vec3 Renderer::getHosekSkySunDirection() const {
  return config.hosek_sky_sun_direciton;
}
Real Renderer::getHosekSkyTurbidity() const {
  return config.hosek_sky_turbidity;
}
Vec3 Renderer::getHosekSkyAlbedo() const { return config.hosek_sky_albedo; }
void Renderer::setHosekSkySunDirection(const Vec3& sun_direction) {
  config.hosek_sky_sun_direciton = sun_direction;
}
void Renderer::setHosekSkyTurbidity(const Real& turbidity) {
  config.hosek_sky_turbidity = turbidity;
}

void Renderer::getLayersRGB(std::vector<float>& rgb) const {
  if (config.layer_type == LayerType::Render) {
    getRendersRGB(rgb);
  } else if (config.layer_type == LayerType::Normal) {
    getNormalsRGB(rgb);
  } else if (config.layer_type == LayerType::Position) {
    getPositionsRGB(rgb);
  } else if (config.layer_type == LayerType::Depth) {
    getDepthsRGB(rgb);
  } else if (config.layer_type == LayerType::Sample) {
    getSamplesRGB(rgb);
  }
}

void Renderer::saveLayer(const std::string& filename) const {
  std::vector<float> image;
  getLayersRGB(image);

  if (config.image_type == ImageType::PPM) {
    writePPM(filename, config.width, config.height, image);
  } else if (config.image_type == ImageType::PNG) {
    writePNG(filename, config.width, config.height, image);
  } else if (config.image_type == ImageType::EXR) {
    writeEXR(filename, config.width, config.height, image);
  } else if (config.image_type == ImageType::HDR) {
    writeHDR(filename, config.width, config.height, image);
  } else {
    std::cerr << "nvalid image type" << std::endl;
  }
}

void Renderer::getRendersRGB(std::vector<float>& rgb) const {
  rgb.resize(3 * config.width * config.height);

  for (int j = 0; j < config.height; ++j) {
    for (int i = 0; i < config.width; ++i) {
      const int index = 3 * i + 3 * config.width * j;

      RGB rgb_vec =
          RGB(layer.render_sRGB[index + 0], layer.render_sRGB[index + 1],
              layer.render_sRGB[index + 2]);

      // Tone Mapping
      // RGB to luminance
      // https://imdoingitwrong.wordpress.com/tag/tone-mapping/
      const float luminance =
          0.2126 * rgb_vec[0] + 0.7152 * rgb_vec[1] + 0.0722 * rgb_vec[2];
      if (config.tone_mapping_type == ToneMappingType::Reinhard) {
        rgb_vec *= reinhardToneMapping(luminance, config.exposure) / luminance;
      }

      // ガンマ補正
      rgb_vec[0] = std::pow(rgb_vec[0], 1 / config.gamma);
      rgb_vec[1] = std::pow(rgb_vec[1], 1 / config.gamma);
      rgb_vec[2] = std::pow(rgb_vec[2], 1 / config.gamma);

      rgb[index + 0] = rgb_vec.x();
      rgb[index + 1] = rgb_vec.y();
      rgb[index + 2] = rgb_vec.z();
    }
  }
}

void Renderer::getNormalsRGB(std::vector<float>& rgb) const {
  rgb.resize(3 * config.width * config.height);

  for (int j = 0; j < config.height; ++j) {
    for (int i = 0; i < config.width; ++i) {
      const int index = 3 * i + 3 * config.width * j;
      rgb[index + 0] = layer.normal_sRGB[index + 0];
      rgb[index + 1] = layer.normal_sRGB[index + 1];
      rgb[index + 2] = layer.normal_sRGB[index + 2];
    }
  }
}

void Renderer::getPositionsRGB(std::vector<float>& rgb) const {
  rgb.resize(3 * config.width * config.height);

  for (int j = 0; j < config.height; ++j) {
    for (int i = 0; i < config.width; ++i) {
      const int index = 3 * i + 3 * config.width * j;
      rgb[index + 0] = layer.position_sRGB[index + 0];
      rgb[index + 1] = layer.position_sRGB[index + 1];
      rgb[index + 2] = layer.position_sRGB[index + 2];
    }
  }
}

void Renderer::getDepthsRGB(std::vector<float>& rgb) const {
  rgb.resize(3 * config.width * config.height);

  for (int j = 0; j < config.height; ++j) {
    for (int i = 0; i < config.width; ++i) {
      const int index = 3 * i + 3 * config.width * j;
      rgb[index + 0] = layer.depth_sRGB[index + 0];
      rgb[index + 1] = layer.depth_sRGB[index + 1];
      rgb[index + 2] = layer.depth_sRGB[index + 2];
    }
  }
}

void Renderer::getSamplesRGB(std::vector<float>& rgb) const {
  rgb.resize(3 * config.width * config.height);

  for (int j = 0; j < config.height; ++j) {
    for (int i = 0; i < config.width; ++i) {
      const int index = 3 * i + 3 * config.width * j;
      rgb[index + 0] = layer.sample_sRGB[index + 0];
      rgb[index + 1] = layer.sample_sRGB[index + 1];
      rgb[index + 2] = layer.sample_sRGB[index + 2];
    }
  }
}

}  // namespace Prl2