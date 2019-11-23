#ifndef VEC3_H
#define VEC3_H

#include <cassert>
#include <cmath>
#include <iostream>

#include "core/type.h"

namespace Prl2 {

class alignas(16) Vec3 {
 public:
  Vec3() { v[0] = v[1] = v[2] = 0; };
  Vec3(const Real& _x) {
    assert(!std::isnan(_x));
    v[0] = v[1] = v[2] = _x;
  };
  Vec3(const Real& _x, const Real& _y, const Real& _z) {
    assert(!std::isnan(_x) && !std::isnan(_y) && std::isnan(_z));
    v[0] = _x;
    v[1] = _y;
    v[2] = _z;
  };

  Real& operator[](int i) {
    assert(i >= 0 && i < 3);
    return v[i];
  };

  Real x() const { return v[0]; };
  Real y() const { return v[1]; };
  Real z() const { return v[2]; };

  Vec3 operator-() const { return Vec3(-x(), -y(), -z()); };

  Vec3& operator+=(const Vec3& v2) {
    v[0] += v2.x();
    v[1] += v2.y();
    v[2] += v2.z();
    return *this;
  };
  Vec3& operator+=(const Real& k) {
    v[0] += k;
    v[1] += k;
    v[2] += k;
    return *this;
  };
  Vec3& operator-=(const Vec3& v2) {
    v[0] -= v2.x();
    v[1] -= v2.y();
    v[2] -= v2.z();
    return *this;
  };
  Vec3& operator-=(const Real& k) {
    v[0] -= k;
    v[1] -= k;
    v[2] -= k;
    return *this;
  };
  Vec3& operator*=(const Vec3& v2) {
    v[0] *= v2.x();
    v[1] *= v2.y();
    v[2] *= v2.z();
    return *this;
  };
  Vec3& operator*=(const Real& k) {
    v[0] *= k;
    v[1] *= k;
    v[2] *= k;
    return *this;
  };
  Vec3& operator/=(const Vec3& v2) {
    v[0] /= v2.x();
    v[1] /= v2.y();
    v[2] /= v2.z();
    return *this;
  };
  Vec3& operator/=(const Real& k) {
    v[0] /= k;
    v[1] /= k;
    v[2] /= k;
    return *this;
  };

 private:
  Real v[3];
};

inline Vec3 operator+(const Vec3& v1, const Vec3& v2) {
  return Vec3(v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z());
}
inline Vec3 operator+(const Vec3& v, const Real& k) {
  return Vec3(v.x() + k, v.y() + k, v.z() + k);
}
inline Vec3 operator+(const Real& k, const Vec3& v) {
  return Vec3(k + v.x(), k + v.y(), k + v.z());
}

inline Vec3 operator-(const Vec3& v1, const Vec3& v2) {
  return Vec3(v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z());
}
inline Vec3 operator-(const Vec3& v, const Real& k) {
  return Vec3(v.x() - k, v.y() - k, v.z() - k);
}
inline Vec3 operator-(const Real& k, const Vec3& v) {
  return Vec3(k - v.x(), k - v.y(), k - v.z());
}

inline Vec3 operator*(const Vec3& v1, const Vec3& v2) {
  return Vec3(v1.x() * v2.x(), v1.y() * v2.y(), v1.z() * v2.z());
}
inline Vec3 operator*(const Vec3& v, const Real& k) {
  return Vec3(v.x() * k, v.y() * k, v.z() * k);
}
inline Vec3 operator*(const Real& k, const Vec3& v) {
  return Vec3(k * v.x(), k * v.y(), k * v.z());
}

inline Vec3 operator/(const Vec3& v1, const Vec3& v2) {
  return Vec3(v1.x() / v2.x(), v1.y() / v2.y(), v1.z() / v2.z());
}
inline Vec3 operator/(const Vec3& v, const Real& k) {
  return Vec3(v.x() / k, v.y() / k, v.z() / k);
}
inline Vec3 operator/(const Real& k, const Vec3& v) {
  return Vec3(k / v.x(), k / v.y(), k / v.z());
}

inline Real length(const Vec3& v) {
  return std::sqrt(v.x() * v.x() + v.y() * v.y() + v.z() * v.z());
}
inline Real length2(const Vec3& v) {
  return v.x() * v.x() + v.y() * v.y() + v.z() * v.z();
}

inline Real dot(const Vec3& v1, const Vec3& v2) {
  return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
}
inline Vec3 cross(const Vec3& v1, const Vec3& v2) {
  return Vec3(v1.y() * v2.z() - v1.z() * v2.y(),
              v1.z() * v2.x() - v1.x() * v2.z(),
              v1.x() * v2.y() - v1.y() * v2.x());
}

inline Vec3 normalize(const Vec3& v) { return v / length(v); }

inline std::ostream& operator<<(std::ostream& stream, const Vec3& v) {
  stream << "(" << v.x() << ", " << v.y() << ", " << v.z() << ")";
  return stream;
}

}  // namespace Prl2
#endif
