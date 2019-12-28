#ifndef _PRL2_BOUNDS3_H
#define _PRL2_BOUNDS3_H

#include "core/vec3.h"

namespace Prl2 {

class Bounds3 {
 public:
  const Vec3 p1;
  const Vec3 p2;

  Bounds3(){};
  Bounds3(const Vec3& _p1, const Vec3& _p2) : p1(_p1), p2(_p2){};
};

}  // namespace Prl2

#endif