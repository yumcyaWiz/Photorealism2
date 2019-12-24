#ifndef _PRL2_PLANE_H
#define _PRL2_PLANE_H

#include "shape/shape.h"

namespace Prl2 {

class Plane : public Shape {
 public:
  Plane(){};

  bool intersect(const Ray& ray, IntersectInfo& info) const override;

  void samplePoint(Sampler& sampler, Vec3& p, Vec3& n,
                   Real& pdf_area) const override;
};

}  // namespace Prl2

#endif