#ifndef VEC4_H
#define VEC4_H

#include "type.h"


struct alignas(16) Vec4 {
  Real x;
  Real y;
  Real z;
  Real w;

  Vec4() { x = y = z = w = 0; };
  Vec4(const Real& _x) { x = y = z = w = _x; };
  Vec4(const Real& _x, const Real& _y, const Real& _z, const Real& _w) : x(_x), y(_y), z(_z), w(_w) {};
};

inline Vec4 operator+(const Vec4& v1, const Vec4& v2) {
  return Vec4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
}
inline Vec4 operator-(const Vec4& v1, const Vec4& v2) {
  return Vec4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
}

Real dot(const Vec4& v1, const Vec4& v2) {
  return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
}

#endif