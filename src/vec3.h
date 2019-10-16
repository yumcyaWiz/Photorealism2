#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <cmath>

#include "type.h"

struct alignas(16) Vec3 {
  Real x;
  Real y;
  Real z;

  Vec3() { x = y = z = 0; };
  Vec3(const Real& _x) { x = y = z = _x; };
  Vec3(const Real& _x, const Real& _y, const Real& _z) : x(_x), y(_y), z(_z) {};
};


inline Vec3 operator+(const Vec3& v1, const Vec3& v2) {
  return Vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}
inline Vec3 operator+(const Vec3& v, const Real& k) {
  return Vec3(v.x + k, v.y + k, v.z + k);
}
inline Vec3 operator+(const Real& k, const Vec3& v) {
  return Vec3(k + v.x, k + v.y, k + v.z);
}


inline Vec3 operator-(const Vec3& v1, const Vec3& v2) {
  return Vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}
inline Vec3 operator-(const Vec3& v, const Real& k) {
  return Vec3(v.x - k, v.y - k, v.z - k);
}
inline Vec3 operator-(const Real& k, const Vec3& v) {
  return Vec3(k - v.x, k - v.y, k - v.z);
}


inline Vec3 operator*(const Vec3& v1, const Vec3& v2) {
  return Vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}
inline Vec3 operator*(const Vec3& v, const Real& k) {
  return Vec3(v.x * k, v.y * k, v.z * k);
}
inline Vec3 operator*(const Real& k, const Vec3& v) {
  return Vec3(k * v.x, k * v.y, k * v.z);
}


inline Vec3 operator/(const Vec3& v1, const Vec3& v2) {
  return Vec3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}
inline Vec3 operator/(const Vec3& v, const Real& k) {
  return Vec3(v.x / k, v.y / k, v.z / k);
}
inline Vec3 operator/(const Real& k, const Vec3& v) {
  return Vec3(k / v.x, k / v.y, k / v.z);
}


inline Real length(const Vec3& v) {
  return std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}
inline Real length2(const Vec3& v) {
  return v.x*v.x + v.y*v.y + v.z*v.z;
}


inline Real dot(const Vec3& v1, const Vec3& v2) {
  return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}
inline Vec3 cross(const Vec3& v1, const Vec3& v2) {
  return Vec3(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}


inline Vec3 normalize(const Vec3& v) {
  return v / length(v);
}


inline std::ostream& operator<<(std::ostream& stream, const Vec3& v) {
  stream << "(" << v.x << ", " << v.y << ", " << v.z << ")";
  return stream;
}
#endif
