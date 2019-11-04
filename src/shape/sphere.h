#ifndef SPHERE_H
#define SPHERE_H

#include "shape/shape.h"
#include "type.h"

class Sphere : public Shape {
 public:
  const Real radius;  //半径

  Sphere(const Real& _radius) : radius(_radius){};

  bool intersect(const Ray& ray, IntersectInfo& info) const;
};
#endif