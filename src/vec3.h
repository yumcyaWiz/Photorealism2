#ifndef VEC3_H
#define VEC3_H

#include "type.h"

struct alignas(16) Vec3 {
  Real x;
  Real y;
  Real z;

  Vec3() { x = y = z = 0; };
  Vec3(const Real& _x) { x = y = z = _x; };
  Vec3(const Real& _x, const Real& _y, const Real& _z) : x(_x), y(_y), z(_z) {};
};
#endif
