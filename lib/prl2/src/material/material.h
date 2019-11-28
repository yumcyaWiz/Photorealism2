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

// 反射ベクトルを返す
inline Vec3 reflect(const Vec3& v, const Vec3& n) {
  return -v + 2 * dot(v, n) * n;
}

// フレネル係数を計算する
inline Real fresnel(const Vec3& wo, const Vec3& n, const Real& n1,
                    const Real& n2) {
  const float f0 = std::pow((n1 - n2) / (n1 + n2), 2.0f);
  return f0 + (1.0f - f0) * std::pow(1.0f - dot(wo, n), 5.0f);
}

// 屈折ベクトルを返す
inline bool refract(const Vec3& wi, Vec3& wt, const Vec3& n, const Real& ior1,
                    const Real& ior2) {
  const Real eta = ior1 / ior2;
  const Real cosThetaI = dot(wi, n);
  const Real sin2ThetaI = std::max(0.0f, 1.0f - cosThetaI * cosThetaI);
  const Real sin2ThetaT = eta * eta * sin2ThetaI;
  if (sin2ThetaT >= 1.0f) return false;
  const Real cosThetaT = std::sqrt(1.0f - sin2ThetaT);
  wt = eta * (-wi) + (eta * cosThetaI - cosThetaT) * n;
  return true;
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
