#include "renderer/renderer.h"
#include "sampler/random.h"

namespace Prl2 {

void Renderer::render(const Scene& scene, const RenderConfig& config,
                      RenderLayer& layer) const {
  // それぞれの画素で同じ処理を行う
  for (int j = 0; j < config.height; ++j) {
    for (int i = 0; i < config.width; ++i) {
      //サンプリングを繰り返す
      for (int k = 0; k < config.samples; ++k) {
        //フィルム面のUV座標
        const Real u = (2.0f * (i + 0.0f) - config.width) / config.width;
        const Real v = (2.0f * (j + 0.0f) - config.height) / config.height;

        //カメラからレイを生成
        Ray ray;
        if (scene.camera->generateRay(u, v, ray)) {
          // Primary Rayで描画できるもの
          IntersectInfo info;
          if (scene.intersector->intersect(ray, info)) {
            layer.normal_sRGB[3 * i + 3 * config.width * j + 0] +=
                0.5f * (info.hitNormal.x + 1.0f);
            layer.normal_sRGB[3 * i + 3 * config.width * j + 1] +=
                0.5f * (info.hitNormal.y + 1.0f);
            layer.normal_sRGB[3 * i + 3 * config.width * j + 2] +=
                0.5f * (info.hitNormal.z + 1.0f);
          }
        }
      }
    }
  }
}

};  // namespace Prl2