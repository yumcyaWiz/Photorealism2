#ifndef RENDER_LAYER_H
#define RENDER_LAYER_H

#include <vector>

#include "core/type.h"

namespace Prl2 {

//レンダリング後の画像を格納する
struct RenderLayer {
  RenderLayer(){};

  std::vector<Real> sRGB;  // レンダリング結果のsRGBを[0, 1]の範囲で格納する
  std::vector<Real> normalsRGB;  // 法線をsRGBにしたものを格納する
  std::vector<Real> depthsRGB;   //深度をsRGBにしたものを格納する
  std::vector<Real> positionsRGB;  //位置をsRGBにしたものを格納する
};

}  // namespace Prl2

#endif