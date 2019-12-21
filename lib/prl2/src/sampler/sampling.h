#ifndef SAMPLING_H
#define SAMPLING_H

#include <algorithm>
#include <cmath>

#include "core/constant.h"
#include "core/type.h"
#include "core/vec2.h"
#include "core/vec3.h"

namespace Prl2 {

inline Vec2 sampleDisk(const Vec2& u) {
  const Real r = std::sqrt(u.x());
  const Real theta = PI_MUL_2 * u.y();
  return Vec2(r * std::cos(theta), r * std::sin(theta));
}

inline Vec3 sampleHemisphere(const Vec2& u) {
  const Real z = u.x();
  const Real r = std::sqrt(std::max(0.0f, 1.0f - z * z));
  const Real phi = 2.0 * PI_MUL_2 * u.y();
  return Vec3(r * std::cos(phi), z, r * std::sin(phi));
}

inline Vec3 sampleCosineHemisphere(const Vec2& u) {
  const Vec2 d = sampleDisk(u);
  const Real z = std::sqrt(
      std::max(static_cast<Real>(0), static_cast<Real>(1.0f - u.x())));
  return Vec3(d.x(), z, d.y());
}

}  // namespace Prl2

#endif