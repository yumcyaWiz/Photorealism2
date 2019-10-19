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

  Real& operator[](std::size_t i) {
    if (i == 0) return x;
    else if (i == 1) return y;
    else if (i == 2) return z;
    else if (i == 3) return w;
    else {
      std::cerr << "invalid vec4 index" << std::endl;
      std::exit(EXIT_FAILURE);
    }
  };
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