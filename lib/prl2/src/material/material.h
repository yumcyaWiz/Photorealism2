#ifndef MATERIAL_H
#define MATERIAL_H

#include "core/isect.h"
#include "core/ray.h"
#include "core/type.h"
#include "core/vec3.h"
#include "sampler/sampler.h"

namespace Prl2 {

// 方向ベクトルと法線との間のコサインを計算する
inline Real cosTheta(const Vec3& w) { return w.y(); }
// 方向ベクトルと法線との間のコサインの絶対値を計算する
inline Real absCosTheta(const Vec3& w) { return std::abs(w.y()); }

//ワールド座標系の方向ベクトルをマテリアル座標系の方向ベクトルに変換する
inline Vec3 worldToMaterial(const Vec3& v, const IntersectInfo& info) {
  return Vec3(dot(v, info.dpdu), dot(v, info.hitNormal), dot(v, info.dpdv));
}

//マテリアル座標系の方向ベクトルをワールド座標系に変換する
inline Vec3 materialToWorld(const Vec3& v, const IntersectInfo& info) {
  return v.x() * info.dpdu + v.y() * info.hitNormal + v.z() * info.dpdv;
}

// Materialを表現するクラス
// マテリアル座標系は原点を衝突点、+Xを接線, +Yを法線
// -Zを陪法線とする座標系で定義される
class Material {
 public:
  Material(){};

  //マテリアル座標系で次のレイの方向をサンプリングする
  //評価した分光反射率を返り値とする
  virtual Real sampleDirection(const Vec3& wo_local, const Real& lambda,
                               Sampler& sampler, Vec3& wi_local,
                               Real& pdf) const = 0;
};

}  // namespace Prl2

#endif
