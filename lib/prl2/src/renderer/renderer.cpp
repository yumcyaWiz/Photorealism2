#include <algorithm>
#include <chrono>

#include "camera/environment.h"
#include "camera/pinhole.h"
#include "camera/thin-lens.h"
#include "core/transform.h"
#include "integrator/ao.h"
#include "integrator/nee.h"
#include "integrator/pt.h"
#include "light/light.h"
#include "parallel/parallel.h"
#include "postprocess/tone_mapping.h"
#include "renderer/renderer.h"
#include "renderer/scene-loader.h"
#include "sampler/random.h"
#include "sky/hosek_sky.h"
#include "sky/ibl_sky.h"
#include "sky/uniform_sky.h"

#include "OpenImageDenoise/oidn.h"

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
  /*
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
  */
  integrator = std::make_shared<PT>();
}

void Renderer::renderPixel(int i, int j, Sampler& sampler) {
  // Primary Rayで計算できるものを計算
  {
    Vec2 pFilm = scene.camera->sampleFilm(i, j, sampler);
    Ray ray;
    ray.lambda = 550;
    Real camera_cos, camera_pdf;
    if (scene.camera->generateRay(pFilm, sampler, ray, camera_cos,
                                  camera_pdf)) {
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
        const auto material = info.hitPrimitive->getMaterial();
        const Vec3 wo = -ray.direction;
        const Vec3 wo_local = worldToMaterial(wo, info);
        SurfaceInteraction interaction;
        interaction.wo_local = wo_local;
        interaction.lambda = ray.lambda;
        Real pdf;
        material->sampleDirection(interaction, sampler, pdf);
        const Vec3 wi = materialToWorld(interaction.wi_local, info);
        layer.sample_sRGB[3 * i + 3 * config.width * j + 0] +=
            0.5f * (wi.x() + 1.0f);
        layer.sample_sRGB[3 * i + 3 * config.width * j + 1] +=
            0.5f * (wi.y() + 1.0f);
        layer.sample_sRGB[3 * i + 3 * config.width * j + 2] +=
            0.5f * (wi.z() + 1.0f);

        // Albedo Layerの計算
        const RGB albedo = material->albedoRGB(interaction);
        const Real albedo_max =
            std::max(std::max(albedo.x(), albedo.y()), albedo.z());
        layer.albedo_sRGB[3 * i + 3 * config.width * j + 0] +=
            albedo.x() / albedo_max;
        layer.albedo_sRGB[3 * i + 3 * config.width * j + 1] +=
            albedo.y() / albedo_max;
        layer.albedo_sRGB[3 * i + 3 * config.width * j + 2] +=
            albedo.z() / albedo_max;
      }
    }
  }

  IntegratorResult result;
  if (integrator->integrate(i, j, scene, sampler, result)) {
    if (!std::isnan(result.phi)) {
      // フィルムに分光放射束を加算
      scene.camera->film->addPixel(i, j, result.lambda, result.phi);
    } else {
      std::cerr << "nan detected at (" << i << ", " << j << ")" << std::endl;
    }
  }

  // Render LayerにsRGBを書き込み
  const RGB rgb = scene.camera->film->getPixel(i, j).toRGB();
  layer.render_sRGB[3 * i + 3 * config.width * j] = rgb.x();
  layer.render_sRGB[3 * i + 3 * config.width * j + 1] = rgb.y();
  layer.render_sRGB[3 * i + 3 * config.width * j + 2] = rgb.z();
}

void Renderer::render(const std::atomic<bool>& cancel) {
  // Progressを初期化
  num_rendered_pixels = 0;

  // 時間計測
  decltype(std::chrono::system_clock::now()) start_time, finish_time;

  // 画素ごとにサンプリングを繰り返す場合
  if (!config.render_realtime) {
    // レイヤーを初期化
    layer.clear();

    // フィルムをクリア
    scene.camera->film->clear();

    // サンプル数のセット
    for (int j = 0; j < config.height; ++j) {
      for (int i = 0; i < config.width; ++i) {
        layer.samples[i + config.width * j] = config.samples;
      }
    }

    start_time = std::chrono::system_clock::now();
    pool.parallelFor2D(
        [&](int i, int j) {
          // Samplerを画素ごとに用意する
          const std::unique_ptr<Sampler> pixel_sampler =
              sampler->clone(i + config.width * j);

          //サンプリングを繰り返す
          for (int k = 0; k < config.samples; ++k) {
            if (cancel) {
              break;
            }

            renderPixel(i, j, *pixel_sampler);

            // Progressを加算
            num_rendered_pixels += 1;
          }
        },
        config.render_tiles_x, config.render_tiles_y, config.width,
        config.height);
    finish_time = std::chrono::system_clock::now();
  }
  // 1回のサンプリングで画面全体を描画する場合
  // Interactiveに操作する場合に向いている
  else {
    // Samplerを画素ごとに用意する
    std::vector<std::unique_ptr<Sampler>> samplers(config.width *
                                                   config.height);
    for (int j = 0; j < config.height; ++j) {
      for (int i = 0; i < config.width; ++i) {
        samplers[i + config.width * j] = sampler->clone(i + config.width * j);
      }
    }

    start_time = std::chrono::system_clock::now();
    for (int k = 1; k <= config.samples; ++k) {
      pool.parallelFor2D(
          [&](int i, int j) {
            // Layer, Filmの初期化
            // 各スレッドでkが異なる可能性があるので、ここで初期化処理を行うと見た目が綺麗になる
            if (k == 1) {
              layer.clearPixel(i, j, config.width, config.height);
              scene.camera->film->clearPixel(i, j);
            }

            // 最低でも1回は描画してからキャンセルする
            // 見た目が良くなる
            if (k > 1 && cancel) {
              return;
            }

            // 画素ごとに用意したSamplerの取得
            const std::unique_ptr<Sampler>& pixel_sampler =
                samplers[i + config.width * j];

            renderPixel(i, j, *pixel_sampler);

            // サンプル数を加算
            // サンプル数は1で初期化されているので次のIterationから加算する
            if (k > 1) {
              layer.samples[i + config.width * j]++;
            }

            // Progressを加算
            num_rendered_pixels += 1;
          },
          config.render_tiles_x, config.render_tiles_y, config.width,
          config.height);

      if (cancel) {
        break;
      }
    }
    finish_time = std::chrono::system_clock::now();
  }

  // レンダリングに要した時間をセット
  rendering_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                       finish_time - start_time)
                       .count();
}

void Renderer::denoise() {
  // https://github.com/OpenImageDenoise/oidn
  // Create an Intel Open Image Denoise device
  OIDNDevice device = oidnNewDevice(OIDN_DEVICE_TYPE_DEFAULT);
  oidnCommitDevice(device);

  // Create a denoising filter
  OIDNFilter filter =
      oidnNewFilter(device, "RT");  // generic ray tracing filter
  oidnSetSharedFilterImage(filter, "color", layer.render_sRGB.data(),
                           OIDN_FORMAT_FLOAT3, config.width, config.height, 0,
                           0, 0);
  oidnSetSharedFilterImage(filter, "albedo", layer.albedo_sRGB.data(),
                           OIDN_FORMAT_FLOAT3, config.width, config.height, 0,
                           0,
                           0);  // optional
  oidnSetSharedFilterImage(filter, "normal", layer.normal_sRGB.data(),
                           OIDN_FORMAT_FLOAT3, config.width, config.height, 0,
                           0, 0);  // optional
  oidnSetSharedFilterImage(filter, "output", layer.denoised_sRGB.data(),
                           OIDN_FORMAT_FLOAT3, config.width, config.height, 0,
                           0, 0);
  oidnSetFilter1b(filter, "hdr", true);  // image is HDR
  oidnCommitFilter(filter);

  // Filter the image
  oidnExecuteFilter(filter);

  // Check for errors
  const char* errorMessage;
  if (oidnGetDeviceError(device, &errorMessage) != OIDN_ERROR_NONE)
    printf("Error: %s\n", errorMessage);

  // Cleanup
  oidnReleaseFilter(filter);
  oidnReleaseDevice(device);
}

IntegratorType Renderer::getIntegratorType() const {
  return config.integrator_type;
}
void Renderer::setIntegratorType(const IntegratorType& type) {
  config.integrator_type = type;

  if (type == IntegratorType::PT) {
    integrator = std::make_shared<PT>();
  } else if (type == IntegratorType::NEE) {
    integrator = std::make_shared<NEE>();
  }
}

void Renderer::generatePath(int i, int j, std::vector<Ray>& path) const {
  // Samplerを画素ごとに用意する
  const std::unique_ptr<Sampler> pixel_sampler =
      sampler->clone(i + config.width * j);

  // パスの生成
  IntegratorResult result;
  integrator->integrate(i, j, scene, *pixel_sampler, result);

  path = result.rays;
}

SPD Renderer::getSPD(int i, int j) const {
  return scene.camera->film->getPixel(i, j) /
         layer.samples[i + config.width * j];
}

RGB Renderer::getsRGB(int i, int j) const {
  const Real r = layer.render_sRGB[3 * i + 3 * config.width * j];
  const Real g = layer.render_sRGB[3 * i + 3 * config.width * j + 1];
  const Real b = layer.render_sRGB[3 * i + 3 * config.width * j + 2];
  return RGB(r, g, b);
}

Vec3 Renderer::getNormal(int i, int j) const {
  const int samples = layer.samples[i + config.width * j];
  const Real x =
      2.0f * (layer.normal_sRGB[3 * i + 3 * config.width * j] / samples - 0.5f);
  const Real y =
      2.0f *
      (layer.normal_sRGB[3 * i + 3 * config.width * j + 1] / samples - 0.5f);
  const Real z =
      2.0f *
      (layer.normal_sRGB[3 * i + 3 * config.width * j + 2] / samples - 0.5f);
  return Vec3(x, y, z);
}

Real Renderer::getDepth(int i, int j) const {
  return layer.depth_sRGB[3 * i + 3 * config.width * j] /
         layer.samples[i + config.width * j];
}

CameraType Renderer::getCameraType() const { return config.camera_type; }

void Renderer::setCameraType(const CameraType& type) {
  config.camera_type = type;
}

float Renderer::getPinholeCameraFOV() const {
  return config.camera_pinhole_fov;
}
void Renderer::setPinholeCameraFOV(float fov) {
  config.camera_pinhole_fov = fov;
}

void Renderer::moveCamera(const Vec3& pos_diff) {
  scene.camera->moveCamera(pos_diff);
  scene.camera->getLookAt(config.camera_position, config.camera_lookat);
}

void Renderer::setCameraAroundLookAt(const Real& theta, const Real& phi) {
  scene.camera->setCameraAroundLookAt(theta, phi);
}

void Renderer::rotateCamera(const Vec3& r) {
  scene.camera->rotateCamera(r);
  scene.camera->getLookAt(config.camera_position, config.camera_lookat);
}

bool Renderer::getRenderRealtime() const { return config.render_realtime; }

void Renderer::setRenderRealtime(bool realtime) {
  config.render_realtime = realtime;
}

float Renderer::getThinLensCameraFOV() const {
  return config.camera_thin_lens_fov;
}
void Renderer::setThinLensCameraFOV(float fov) {
  config.camera_thin_lens_fov = fov;
}

float Renderer::getThinLensCameraLensRadius() const {
  return config.camera_thin_lens_radius;
}
void Renderer::setThinLensCameraLensRadius(float radius) {
  config.camera_thin_lens_radius = radius;
}

float Renderer::getThinLensCameraFocusDistance() const {
  return config.camera_thin_lens_focus_distance;
}
void Renderer::setThinLensCameraFocusDistance(float distance) {
  config.camera_thin_lens_focus_distance = distance;
}

Real Renderer::getRenderProgress() const {
  return static_cast<Real>(num_rendered_pixels) /
         (static_cast<Real>(config.width) * config.height * config.samples);
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
  } else if (config.camera_type == CameraType::ThinLens) {
    scene.setCamera(std::make_shared<ThinLensCamera>(
        film, transform, config.camera_thin_lens_fov,
        config.camera_thin_lens_radius,
        config.camera_thin_lens_focus_distance));
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

std::string Renderer::getIBLSkyFilename() const {
  return config.ibl_sky_filename;
}
void Renderer::setIBLSkyFilename(const std::string& filename) {
  config.ibl_sky_filename = filename;
}

Vec3 Renderer::getHosekSkySunDirection() const {
  return config.hosek_sky_sun_direciton;
}
Real Renderer::getHosekSkyTurbidity() const {
  return config.hosek_sky_turbidity;
}
Vec3 Renderer::getHosekSkyAlbedo() const { return config.hosek_sky_albedo; }
void Renderer::setHosekSkyAlbedo(const Vec3& albedo) {
  config.hosek_sky_albedo = albedo;
}
void Renderer::setHosekSkySunDirection(const Vec3& sun_direction) {
  config.hosek_sky_sun_direciton =
      length(sun_direction) == 0 ? sun_direction : normalize(sun_direction);
}
void Renderer::setHosekSkyTurbidity(const Real& turbidity) {
  config.hosek_sky_turbidity = turbidity;
}

void Renderer::getLayersRGB(std::vector<float>& rgb) const {
  if (config.layer_type == LayerType::Render) {
    getRendersRGB(rgb);
  } else if (config.layer_type == LayerType::Denoise) {
    getDenoisesRGB(rgb);
  } else if (config.layer_type == LayerType::Albedo) {
    getAlbedosRGB(rgb);
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
  } else if (config.image_type == ImageType::PFM) {
    writePFM(filename, config.width, config.height, image);
  } else {
    std::cerr << "nvalid image type" << std::endl;
  }
}

void Renderer::getRendersRGB(std::vector<float>& rgb) const {
  rgb.resize(3 * config.width * config.height);

  for (int j = 0; j < config.height; ++j) {
    for (int i = 0; i < config.width; ++i) {
      RGB rgb_vec = RGB(layer.render_sRGB[3 * i + 3 * config.width * j],
                        layer.render_sRGB[3 * i + 3 * config.width * j + 1],
                        layer.render_sRGB[3 * i + 3 * config.width * j + 2]) /
                    layer.samples[i + config.width * j];

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

      rgb[3 * i + 3 * config.width * j + 0] = rgb_vec.x();
      rgb[3 * i + 3 * config.width * j + 1] = rgb_vec.y();
      rgb[3 * i + 3 * config.width * j + 2] = rgb_vec.z();
    }
  }
}

void Renderer::getDenoisesRGB(std::vector<float>& rgb) const {
  rgb.resize(3 * config.width * config.height);

  for (int j = 0; j < config.height; ++j) {
    for (int i = 0; i < config.width; ++i) {
      RGB rgb_vec = RGB(layer.denoised_sRGB[3 * i + 3 * config.width * j],
                        layer.denoised_sRGB[3 * i + 3 * config.width * j + 1],
                        layer.denoised_sRGB[3 * i + 3 * config.width * j + 2]) /
                    layer.samples[i + config.width * j];

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

      rgb[3 * i + 3 * config.width * j + 0] = rgb_vec.x();
      rgb[3 * i + 3 * config.width * j + 1] = rgb_vec.y();
      rgb[3 * i + 3 * config.width * j + 2] = rgb_vec.z();
    }
  }
}

void Renderer::getAlbedosRGB(std::vector<float>& rgb) const {
  rgb.resize(3 * config.width * config.height);

  for (int j = 0; j < config.height; ++j) {
    for (int i = 0; i < config.width; ++i) {
      const int index = 3 * i + 3 * config.width * j;
      const int current_samples = layer.samples[i + config.width * j];
      rgb[index + 0] = layer.albedo_sRGB[index + 0] / current_samples;
      rgb[index + 1] = layer.albedo_sRGB[index + 1] / current_samples;
      rgb[index + 2] = layer.albedo_sRGB[index + 2] / current_samples;
    }
  }
}

void Renderer::getNormalsRGB(std::vector<float>& rgb) const {
  rgb.resize(3 * config.width * config.height);

  for (int j = 0; j < config.height; ++j) {
    for (int i = 0; i < config.width; ++i) {
      const int index = 3 * i + 3 * config.width * j;
      const int current_samples = layer.samples[i + config.width * j];
      rgb[index + 0] = layer.normal_sRGB[index + 0] / current_samples;
      rgb[index + 1] = layer.normal_sRGB[index + 1] / current_samples;
      rgb[index + 2] = layer.normal_sRGB[index + 2] / current_samples;
    }
  }
}

void Renderer::getPositionsRGB(std::vector<float>& rgb) const {
  rgb.resize(3 * config.width * config.height);

  for (int j = 0; j < config.height; ++j) {
    for (int i = 0; i < config.width; ++i) {
      const int index = 3 * i + 3 * config.width * j;
      const int current_samples = layer.samples[i + config.width * j];
      rgb[index + 0] = layer.position_sRGB[index + 0] / current_samples;
      rgb[index + 1] = layer.position_sRGB[index + 1] / current_samples;
      rgb[index + 2] = layer.position_sRGB[index + 2] / current_samples;
    }
  }
}

void Renderer::getDepthsRGB(std::vector<float>& rgb) const {
  rgb.resize(3 * config.width * config.height);

  for (int j = 0; j < config.height; ++j) {
    for (int i = 0; i < config.width; ++i) {
      const int index = 3 * i + 3 * config.width * j;
      const int current_samples = layer.samples[i + config.width * j];
      rgb[index + 0] = layer.depth_sRGB[index + 0] / current_samples;
      rgb[index + 1] = layer.depth_sRGB[index + 1] / current_samples;
      rgb[index + 2] = layer.depth_sRGB[index + 2] / current_samples;
    }
  }
}

void Renderer::getSamplesRGB(std::vector<float>& rgb) const {
  rgb.resize(3 * config.width * config.height);

  for (int j = 0; j < config.height; ++j) {
    for (int i = 0; i < config.width; ++i) {
      const int index = 3 * i + 3 * config.width * j;
      const int current_samples = layer.samples[i + config.width * j];
      rgb[index + 0] = layer.sample_sRGB[index + 0] / current_samples;
      rgb[index + 1] = layer.sample_sRGB[index + 1] / current_samples;
      rgb[index + 2] = layer.sample_sRGB[index + 2] / current_samples;
    }
  }
}

void Renderer::getCameraMatrix(Mat4& mat) const {
  scene.camera->getTransformMatrix(mat);
}

}  // namespace Prl2