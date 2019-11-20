#include "renderer/renderer.h"
#include "sampler/random.h"

namespace Prl2 {

void Renderer::render(const Integrator& integrator, const Scene& scene,
                      const RenderConfig& config, RenderLayer& layer) const {
  // それぞれの画素で同じ処理を行う
  for (int j = 0; j < config.height; ++j) {
    for (int i = 0; i < config.width; ++i) {
      // Samplerの初期化
      RandomSampler sampler(i + config.width * j);

      //サンプリングを繰り返す
      for (int k = 0; k < config.samples; ++k) {
        // 波長のサンプリング
        const Real lambda =
            SPD::LAMBDA_MIN +
            0.99f * sampler.getNext() * (SPD::LAMBDA_MAX - SPD::LAMBDA_MIN);

        // サンプリングされた波長をセット
        Ray ray;
        ray.lambda = lambda;

        //フィルム面のUV座標
        const Real u =
            (2.0f * (i + sampler.getNext()) - config.width) / config.width;
        const Real v =
            (2.0f * (j + sampler.getNext()) - config.height) / config.height;

        //カメラからレイを生成
        if (scene.camera->generateRay(u, v, ray)) {
          // Primary Rayで描画できるもの
          IntersectInfo info;
          if (scene.intersector->intersect(ray, info)) {
            // Normal
            layer.normal_sRGB[3 * i + 3 * config.width * j + 0] +=
                0.5f * (info.hitNormal.x() + 1.0f);
            layer.normal_sRGB[3 * i + 3 * config.width * j + 1] +=
                0.5f * (info.hitNormal.y() + 1.0f);
            layer.normal_sRGB[3 * i + 3 * config.width * j + 2] +=
                0.5f * (info.hitNormal.z() + 1.0f);

            // Depth
            layer.depth_sRGB[3 * i + 3 * config.width * j + 0] += info.t;
            layer.depth_sRGB[3 * i + 3 * config.width * j + 1] += info.t;
            layer.depth_sRGB[3 * i + 3 * config.width * j + 2] += info.t;

            // Position
            layer.position_sRGB[3 * i + 3 * config.width * j + 0] +=
                info.hitPos.x();
            layer.position_sRGB[3 * i + 3 * config.width * j + 1] +=
                info.hitPos.y();
            layer.position_sRGB[3 * i + 3 * config.width * j + 2] +=
                info.hitPos.z();
          }

          // 分光放射束の計算
          const Real phi = integrator.integrate(ray, scene, sampler);

          // フィルムに寄与の追加
          scene.camera->film->addPixel(i, j, lambda, phi);
        }
      }
    }
  }
}

};  // namespace Prl2