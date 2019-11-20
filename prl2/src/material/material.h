#ifndef MATERIAL_H
#define MATERIAL_H

#include "core/isect.h"
#include "core/ray.h"
#include "core/type.h"
#include "core/vec3.h"
#include "material/surface_interaction.h"
#include "sampler/sampler.h"

namespace Prl2 {
// Materialを表現するクラス
// マテリアル座標系は原点を衝突点、+Xを接線, +Yを法線
// -Zを陪法線とする座標系で定義される
class Material {
 public:
  Material(){};

  //次のレイの方向をサンプリングする
  //評価した分光反射率を返り値とする
  virtual Real sampleDirection(SurfaceInteraction& interaction,
                               Sampler& sampler, Real& pdf) const = 0;
};

inline Real cosTheta(const Vec3& w) { return w.y(); }
inline Real absCosTheta(const Vec3& w) { return std::abs(w.y()); }

}  // namespace Prl2

#endif
