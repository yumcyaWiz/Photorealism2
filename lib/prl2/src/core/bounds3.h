#ifndef PRL2_BOUNDS3_H
#define PRL2_BOUNDS3_H

#include <iostream>

#include "core/vec3.h"

namespace Prl2 {

class Bounds3 {
 public:
  Vec3 p0;
  Vec3 p1;

  Bounds3() {}
  Bounds3(const Vec3& _p1, const Vec3& _p2) {
    p0[0] = std::min(_p1.x(), _p2.x());
    p0[1] = std::min(_p1.y(), _p2.y());
    p0[2] = std::min(_p1.z(), _p2.z());

    p1[0] = std::max(_p1.x(), _p2.x());
    p1[1] = std::max(_p1.y(), _p2.y());
    p1[2] = std::max(_p1.z(), _p2.z());
  }
};

inline std::ostream& operator<<(std::ostream& stream, const Bounds3& bounds) {
  stream << "p0: " << bounds.p0 << ", p1: " << bounds.p1;
  return stream;
}

}  // namespace Prl2

#endif