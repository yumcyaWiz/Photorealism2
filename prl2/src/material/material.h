#ifndef MATERIAL_H
#define MATERIAL_H

#include "core/isect.h"
#include "core/ray.h"
#include "core/type.h"
#include "core/vec3.h"

namespace Prl2 {
// Materialを表現するクラス
// マテリアル座標系は原点を衝突点、+Xを接線, +Yを法線
// -Zを陪法線とする座標系で定義される
class Material {
 public:
  Material(){};

  //次のレイの方向をサンプリングする
  //評価した分光反射率を返り値とする
  virtual Real sampleDirection(const Ray& ray_in, const IntersectInfo& info,
                               Ray& ray_out, Real& pdf) const = 0;
};

//ワールド座標系の方向ベクトルをマテリアル座標系の方向ベクトルに変換する
inline Vec3 worldToMaterial(const Vec3& v, const IntersectInfo& info) {
  return Vec3(dot(v.x(), info.dpdu), dot(v.y(), info.hitNormal),
              dot(v.z(), info.dpdv));
}

//マテリアル座標系の方向ベクトルをワールド座標系に変換する
inline Vec3 materialToLocal(const Vec3& v, const IntersectInfo& info) {
  return v.x() * info.dpdu + v.y() * info.hitNormal + v.z() * info.dpdv;
}

}  // namespace Prl2

#endif
