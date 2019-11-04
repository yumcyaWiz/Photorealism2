#ifndef ISECT_H
#define ISECT_H

#include "ray.h"
#include "type.h"
#include "vec3.h"

struct IntersectInfo {
  Real t;
  Vec3 hitPos;
  Vec3 hitNormal;

  IntersectInfo() : t(0), hitPos(Vec3()), hitNormal(Vec3()){};
  IntersectInfo(const Real& _t, const Vec3& _hitPos, const Vec3& _hitNormal)
      : t(_t), hitPos(_hitPos), hitNormal(_hitNormal){};
};

#endif