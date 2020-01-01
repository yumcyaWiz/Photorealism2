#ifndef RENDER_LAYER_H
#define RENDER_LAYER_H

#include <algorithm>
#include <vector>

#include "core/type.h"
#include "renderer/render-config.h"

namespace Prl2 {

//レンダリング後の画像を格納する
struct RenderLayer {
  RenderLayer(){};

  // RenderConfigから画像を初期化
  RenderLayer(const RenderConfig& config);

  // リサイズする
  void resize(int width, int height);

  // クリアする
  void clear();

  // 指定したピクセルのデータだけクリアする
  void clearPixel(int i, int j, int width, int height);

  std::vector<Real> render_sRGB;  // レンダリング結果をsRGBにしたものを格納する
  std::vector<Real> denoised_sRGB;  // デノイズされたsRGBを格納する
  std::vector<Real> albedo_sRGB;  // AlbedoをsRGBにしたものを格納する
  std::vector<Real> normal_sRGB;  // 法線をsRGBにしたものを格納する
  std::vector<Real> uv_sRGB;  // UV座標をsRGBにしたものを格納する
  std::vector<Real> depth_sRGB;  // 深度をsRGBにしたものを格納する
  std::vector<Real> position_sRGB;  // 位置をsRGBにしたものを格納する
  std::vector<int> samples;         // サンプル数を格納する
  std::vector<Real>
      sample_sRGB;  // 最初のサンプリング方向をsRGBにしたものを格納する
};

}  // namespace Prl2

#endif