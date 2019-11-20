#include "renderer/renderer.h"
#include "sampler/random.h"

namespace Prl2 {

Renderer::Renderer(const RenderConfig& _config) : config(_config) {}

void Renderer::render(RenderLayer& layer) const {
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
            sampler.getNext() * (SPD::LAMBDA_MAX - SPD::LAMBDA_MIN);

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
          const Real phi = integrator->integrate(ray, scene, sampler);

          // フィルムに分光放射束を加算
          scene.camera->film->addPixel(i, j, lambda, phi);
        }
      }
      // Render LayerにsRGBを加算
      const Vec3 rgb = scene.camera->film->getPixel(i, j).toRGB();
      layer.render_sRGB[3 * i + 3 * config.width * j + 0] = rgb.x();
      layer.render_sRGB[3 * i + 3 * config.width * j + 1] = rgb.y();
      layer.render_sRGB[3 * i + 3 * config.width * j + 2] = rgb.z();
    }
  }
}

}  // namespace Prl2