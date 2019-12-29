#ifndef _PRL2_BOUNDS3_H
#define _PRL2_BOUNDS3_H

#include <iostream>

#include "core/vec3.h"

namespace Prl2 {

class Bounds3 {
 public:
  Vec3 p1;
  Vec3 p2;

  Bounds3(){};
  Bounds3(const Vec3& _p1, const Vec3& _p2) {
    p1[0] = std::min(_p1.x(), _p2.x());
    p1[1] = std::min(_p1.y(), _p2.y());
    p1[2] = std::min(_p1.z(), _p2.z());

    p2[0] = std::max(_p1.x(), _p2.x());
    p2[1] = std::max(_p1.y(), _p2.y());
    p2[2] = std::max(_p1.z(), _p2.z());
  };
};

inline std::ostream& operator<<(std::ostream& stream, const Bounds3& bounds) {
  stream << "p1: " << bounds.p1 << ", p2: " << bounds.p2;
  return stream;
}

}  // namespace Prl2

#endif