#ifndef RAY_H
#define RAY_H

#include "type.h"
#include "vec3.h"

struct alignas(32) Ray {
  Vec3 origin;
  Vec3 direction;

  Ray() {};
  Ray(const Vec3& _origin, const Vec3& _direction) : origin(_origin), direction(_direction) {};

  Vec3 operator()(const Real& t) const {
    return origin + t*direction;
  };
};

#endif