#ifndef RENDER_LAYER_H
#define RENDER_LAYER_H

#include <algorithm>
#include <vector>

#include "core/type.h"
#include "renderer/render_config.h"

namespace Prl2 {

//レンダリング後の画像を格納する
struct RenderLayer {
  RenderLayer(){};

  // RenderConfigから画像を初期化
  RenderLayer(const RenderConfig& config) {
    albedo_sRGB.resize(3 * config.width * config.height);
    normal_sRGB.resize(3 * config.width * config.height);
    depth_sRGB.resize(3 * config.width * config.height);
    position_sRGB.resize(3 * config.width * config.height);
    sample_sRGB.resize(3 * config.width * config.height);
  };

  // リサイズする
  void resize(int width, int height) {
    albedo_sRGB.resize(3 * width * height);
    normal_sRGB.resize(3 * width * height);
    depth_sRGB.resize(3 * width * height);
    position_sRGB.resize(3 * width * height);
    sample_sRGB.resize(3 * width * height);
  };

  // クリアする
  void clear() {
    std::fill(albedo_sRGB.begin(), albedo_sRGB.end(), 0);
    std::fill(normal_sRGB.begin(), normal_sRGB.end(), 0);
    std::fill(depth_sRGB.begin(), depth_sRGB.end(), 0);
    std::fill(position_sRGB.begin(), position_sRGB.end(), 0);
    std::fill(sample_sRGB.begin(), sample_sRGB.end(), 0);
  };

  std::vector<Real> albedo_sRGB;  // AlbedoをsRGBにしたものを格納する
  std::vector<Real> normal_sRGB;  // 法線をsRGBにしたものを格納する
  std::vector<Real> depth_sRGB;   // 深度をsRGBにしたものを格納する
  std::vector<Real> position_sRGB;  // 位置をsRGBにしたものを格納する
  std::vector<Real>
      sample_sRGB;  // 最初のサンプリング方向をsRGBにしたものを格納する
};

}  // namespace Prl2

#endif