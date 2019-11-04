#ifndef RAY_H
#define RAY_H

#include "core/type.h"
#include "core/vec3.h"

namespace Prl2 {

struct alignas(32) Ray {
  Vec3 origin;     //始点
  Vec3 direction;  //方向
  Real lambda;     //波長

  static constexpr Real tmin = 0.001f;    //最小衝突距離
  static constexpr Real tmax = 10000.0f;  //最大衝突距離

  Ray(){};
  Ray(const Vec3& _origin, const Vec3& _direction, const Real& _lambda)
      : origin(_origin), direction(_direction), lambda(_lambda){};

  Vec3 operator()(const Real& t) const { return origin + t * direction; };
};

}  // namespace Prl2

#endif