#include <algorithm>

#include "camera/pinhole.h"
#include "core/transform.h"
#include "integrator/pt.h"
#include "parallel/parallel.h"
#include "renderer/renderer.h"
#include "renderer/scene_loader.h"
#include "sampler/random.h"

namespace Prl2 {

void Renderer::loadConfig(const RenderConfig& _config) {
  // RenderConfigのセット
  config = _config;

  // シーンファイルの読み込み
  if (!config.scene_file.empty()) {
    SceneLoader sceneLoader;
    sceneLoader.loadSceneFromToml(config.scene_file, scene);
  }

  // Filmの設定
  const auto film = std::make_shared<Film>(
      config.width, config.height, config.width_length, config.height_length);

  // Cameraの設定
  std::shared_ptr<Camera> camera = nullptr;
  const auto camera_transform =
      std::make_shared<Transform>(translate(Vec3(0, 0, 0)));
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

void Renderer::render(RenderLayer& layer,
                      const std::atomic<bool>& cancel) const {
  // レイヤーを初期化
  layer.resize(config.width, config.height);

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
            }

            // 分光放射束の計算
            const Real phi = integrator->integrate(ray, scene, *sampler);

            // フィルムに分光放射束を加算
            scene.camera->film->addPixel(i, j, lambda, phi);
          }
        }

        // Render LayerにsRGBを加算
        const Vec3 rgb = scene.camera->film->getPixel(i, j).toRGB();
        layer.render_sRGB[3 * i + 3 * config.width * j + 0] =
            rgb.x() / config.samples;
        layer.render_sRGB[3 * i + 3 * config.width * j + 1] =
            rgb.y() / config.samples;
        layer.render_sRGB[3 * i + 3 * config.width * j + 2] =
            rgb.z() / config.samples;

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
      },
      16, 16, config.width, config.height);
}

}  // namespace Prl2