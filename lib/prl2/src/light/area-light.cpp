#include "light/area-light.h"

namespace Prl2 {

AreaLight::AreaLight(const SPD& _spd, const std::shared_ptr<Shape>& _shape)
    : Light(_spd), shape(_shape) {}

Real AreaLight::Le(const Ray& ray, const IntersectInfo& info) const {
  return spd.sample(ray.lambda);
}

}  // namespace Prl2
