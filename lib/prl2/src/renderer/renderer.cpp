#include <algorithm>

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
  std::shared_ptr<Sky> sky = nullptr;
  if (!config.sky_type.empty()) {
    sky = std::make_shared<UniformSky>(D65Light());
  } else {
    sky = std::make_shared<HosekSky>(4, RGB2Spectrum(RGB(1)));
  }
  scene.sky = sky;

  // Filmの設定
  const auto film = std::make_shared<Film>(
      config.width, config.height, config.width_length, config.height_length);

  // Cameraの設定
  std::shared_ptr<Camera> camera = nullptr;
  const auto camera_transform = std::make_shared<Transform>(
      lookAt(config.camera_position, config.camera_lookat));
  if (!config.camera_type.empty()) {
    camera =
        std::make_shared<PinholeCamera>(film, camera_transform, config.fov);
  } else {
    camera =
        std::make_shared<PinholeCamera>(film, camera_transform, config.fov);
  }
  scene.camera = camera;

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
  // レイヤーを初期化
  layer.clear();

  // フィルムをクリア
  scene.camera->film->clear();

  // それぞれの画素で同じ処理を行う
  parallelFor2D(
      [&](int i, int j) {
        // Samplerの初期化
        sampler->setSeed(i + config.width * j);

        //サンプリングを繰り返す
        for (int k = 0; k < config.samples; ++k) {
          if (cancel) {
            break;
          }

          // 波長のサンプリング
          const Real lambda =
              SPD::LAMBDA_MIN +
              sampler->getNext() * (SPD::LAMBDA_MAX - SPD::LAMBDA_MIN);
          constexpr Real lambda_pdf = 1 / (SPD::LAMBDA_MAX - SPD::LAMBDA_MIN);

          // サンプリングされた波長をセット
          Ray ray;
          ray.lambda = lambda;

          //フィルム面のUV座標
          const Real u =
              (2.0f * (i + sampler->getNext()) - config.width) / config.width;
          const Real v =
              (2.0f * (j + sampler->getNext()) - config.height) / config.height;

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
              Vec3 wi_local;
              Real pdf;
              material->sampleDirection(wo_local, ray.lambda, *sampler,
                                        wi_local, pdf);
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
                integrator->integrate(ray, scene, *sampler) / lambda_pdf;

            // フィルムに分光放射束を加算
            scene.camera->film->addPixel(i, j, lambda, phi);
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

        layer.position_sRGB[3 * i + 3 * config.width * j + 0] /= config.samples;
        layer.position_sRGB[3 * i + 3 * config.width * j + 1] /= config.samples;
        layer.position_sRGB[3 * i + 3 * config.width * j + 2] /= config.samples;

        layer.sample_sRGB[3 * i + 3 * config.width * j + 0] /= config.samples;
        layer.sample_sRGB[3 * i + 3 * config.width * j + 1] /= config.samples;
        layer.sample_sRGB[3 * i + 3 * config.width * j + 2] /= config.samples;
      },
      16, 16, config.width, config.height);
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