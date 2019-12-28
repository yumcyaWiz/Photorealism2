#ifndef SPHERE_H
#define SPHERE_H

#include "core/type.h"
#include "shape/shape.h"

namespace Prl2 {

// 単位球を表現するクラス
class Sphere : public Shape {
 public:
  Sphere(){};

  bool intersect(const Ray& ray, IntersectInfo& info) const override;

  Bounds3 getBounds() const override;

  void samplePoint(Sampler& sampler, Vec3& p, Vec3& n,
                   Real& pdf_area) const override;
};

}  // namespace Prl2
#endif