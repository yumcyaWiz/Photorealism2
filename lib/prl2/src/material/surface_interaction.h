#ifndef SURFACE_INTERACTION
#define SURFACE_INTERACTION

#include "core/isect.h"
#include "core/ray.h"
#include "core/type.h"
#include "core/vec3.h"

namespace Prl2 {

//ワールド座標系の方向ベクトルをマテリアル座標系の方向ベクトルに変換する
inline Vec3 worldToMaterial(const Vec3& v, const IntersectInfo& info) {
  return Vec3(dot(v.x(), info.dpdu), dot(v.y(), info.hitNormal),
              dot(v.z(), info.dpdv));
}

//マテリアル座標系の方向ベクトルをワールド座標系に変換する
inline Vec3 materialToWorld(const Vec3& v, const IntersectInfo& info) {
  return v.x() * info.dpdu + v.y() * info.hitNormal + v.z() * info.dpdv;
}

class SurfaceInteraction {
 public:
  Vec3 wi;      //入射方向
  Vec3 wo;      //出射方向
  Vec3 n;       //法線
  Real lambda;  //波長

  SurfaceInteraction(){};
  SurfaceInteraction(const Ray& ray, const IntersectInfo& info) {
    wi = worldToMaterial(-ray.direction, info);
    n = info.hitNormal;
    lambda = ray.lambda;
  }
};

}  // namespace Prl2

#endif