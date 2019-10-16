#ifndef VEC2_H
#define VEC2_H

#include <iostream>
#include <cmath>

#include "type.h"

struct alignas(8) Vec2 {
  Real x;
  Real y;

  Vec2() { x = y = 0; };
  Vec2(const Real& _x) { x = y = _x; };
  Vec2(const Real& _x, const Real& _y) : x(_x), y(_y) {};
};


inline Vec2 operator+(const Vec2& v1, const Vec2& v2) {
  return Vec2(v1.x + v2.x, v1.y + v2.y);
}
inline Vec2 operator+(const Vec2& v, const Real& k) {
  return Vec2(v.x + k, v.y + k);
}
inline Vec2 operator+(const Real& k, const Vec2& v) {
  return Vec2(k + v.x, k + v.y);
}


inline Vec2 operator-(const Vec2& v1, const Vec2& v2) {
  return Vec2(v1.x - v2.x, v1.y - v2.y);
}
inline Vec2 operator-(const Vec2& v, const Real& k) {
  return Vec2(v.x - k, v.y - k);
}
inline Vec2 operator-(const Real& k, const Vec2& v) {
  return Vec2(k - v.x, k - v.y);
}


inline Vec2 operator*(const Vec2& v1, const Vec2& v2) {
  return Vec2(v1.x * v2.x, v1.y * v2.y);
}
inline Vec2 operator*(const Vec2& v, const Real& k) {
  return Vec2(v.x * k, v.y * k);
}
inline Vec2 operator*(const Real& k, const Vec2& v) {
  return Vec2(k * v.x, k * v.y);
}


inline Vec2 operator/(const Vec2& v1, const Vec2& v2) {
  return Vec2(v1.x / v2.x, v1.y / v2.y);
}
inline Vec2 operator/(const Vec2& v, const Real& k) {
  return Vec2(v.x / k, v.y / k);
}
inline Vec2 operator/(const Real& k, const Vec2& v) {
  return Vec2(k / v.x, k / v.y);
}


inline Real length(const Vec2& v) {
  return std::sqrt(v.x*v.x + v.y*v.y);
}
inline Real length2(const Vec2& v) {
  return v.x*v.x + v.y*v.y;
}


inline Real dot(const Vec2& v1, const Vec2& v2) {
  return v1.x*v2.x + v1.y*v2.y;
}


inline Vec2 normalize(const Vec2& v) {
  return v / length(v);
}

#endif