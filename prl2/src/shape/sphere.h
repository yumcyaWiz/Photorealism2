#ifndef SPHERE_H
#define SPHERE_H

#include "core/type.h"
#include "shape/shape.h"

namespace Prl2 {

class Sphere : public Shape {
 public:
  const Real radius;  //半径

  Sphere(const Real& _radius) : radius(_radius){};

  bool intersect(const Ray& ray, IntersectInfo& info) const;
};

}  // namespace Prl2
#endif