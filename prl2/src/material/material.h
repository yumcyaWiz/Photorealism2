#ifndef MATERIAL_H
#define MATERIAL_H

#include "core/isect.h"
#include "core/ray.h"
#include "core/type.h"

namespace Prl2 {

// Materialを表現するクラス
// Materialにおいて方向ベクトルは原点を衝突点、+Yを法線とする座標系で定義される
class Material {
 public:
  Material(){};

  //次のレイの方向をサンプリングする
  //評価した分光反射率を返り値とする
  virtual Real sampleDirection(const Ray& ray_in, const IntersectInfo& info,
                               Ray& ray_out, Real& pdf) const = 0;
};

}  // namespace Prl2

#endif
