#ifndef PRL2_VEC4_H
#define PRL2_VEC4_H

#include <cassert>
#include <iostream>

#include "core/type.h"

namespace Prl2 {

class alignas(16) Vec4 {
 public:
  explicit Vec4() { v[0] = v[1] = v[2] = v[3] = 0; }
  explicit Vec4(const Real& _x) {
    assert(!std::isnan(_x));
    v[0] = v[1] = v[2] = v[3] = _x;
  }
  explicit Vec4(const Real& _x, const Real& _y, const Real& _z,
                const Real& _w) {
    assert(!std::isnan(_x) && !std::isnan(_y) && !std::isnan(_z) &&
           !std::isnan(_w));
    v[0] = _x;
    v[1] = _y;
    v[2] = _z;
    v[3] = _w;
  }

  Real& operator[](int i) {
    assert(i >= 0 && i < 4);
    return v[i];
  }

  Real x() const { return v[0]; }
  Real y() const { return v[1]; }
  Real z() const { return v[2]; }
  Real w() const { return v[3]; }

 private:
  Real v[4];
};

inline Vec4 operator+(const Vec4& v1, const Vec4& v2) {
  return Vec4(v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z(),
              v1.w() + v2.w());
}

inline Vec4 operator-(const Vec4& v1, const Vec4& v2) {
  return Vec4(v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z(),
              v1.w() - v2.w());
}

inline Real dot(const Vec4& v1, const Vec4& v2) {
  return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z() + v1.w() * v2.w();
}

inline std::ostream& operator<<(std::ostream& stream, const Vec4& v) {
  stream << "(" << v.x() << ", " << v.y() << ", " << v.z() << ", " << v.w()
         << ")";
  return stream;
}

}  // namespace Prl2
#endif