#ifndef SHAPE_H
#define SHAPE_H

#include "isect.h"
#include "ray.h"

class Shape {
 public:
  virtual bool intersect(const Ray& ray, IntersectInfo& info) const = 0;
};

#endif