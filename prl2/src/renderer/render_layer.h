#ifndef RENDER_LAYER_H
#define RENDER_LAYER_H

#include <vector>

#include "core/type.h"
#include "renderer/render_config.h"

namespace Prl2 {

//レンダリング後の画像を格納する
struct RenderLayer {
  RenderLayer(){};

  // RenderConfigから画像を初期化
  RenderLayer(const RenderConfig& config) {
    render_sRGB.resize(3 * config.width * config.height, 0);
    normal_sRGB.resize(3 * config.width * config.height, 0);
    depth_sRGB.resize(3 * config.width * config.height, 0);
    position_sRGB.resize(3 * config.width * config.height, 0);
  };

  // リサイズする
  void resize(int width, int height) {
    render_sRGB.resize(3 * width * height, 0);
    normal_sRGB.resize(3 * width * height, 0);
    depth_sRGB.resize(3 * width * height, 0);
    position_sRGB.resize(3 * width * height, 0);
  };

  // クリアする
  void clear() {
    render_sRGB.clear();
    normal_sRGB.clear();
    depth_sRGB.clear();
    position_sRGB.clear();
  };

  std::vector<Real>
      render_sRGB;  // レンダリング結果のsRGBを[0, 1]の範囲で格納する
  std::vector<Real> normal_sRGB;  // 法線をsRGBにしたものを格納する
  std::vector<Real> depth_sRGB;   //深度をsRGBにしたものを格納する
  std::vector<Real> position_sRGB;  //位置をsRGBにしたものを格納する
};

}  // namespace Prl2

#endif