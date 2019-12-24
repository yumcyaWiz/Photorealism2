#include "light/area-light.h"

namespace Prl2 {

AreaLight::AreaLight(const SPD& _spd,
                     const std::shared_ptr<Geometry>& _geometry)
    : Light(_spd), geometry(_geometry) {}

Real AreaLight::Le(const Ray& ray, const IntersectInfo& info) const {
  if (dot(-ray.direction, info.hitNormal) > 0) {
    return spd.sample(ray.lambda);
  } else {
    return 0;
  }
}

}  // namespace Prl2
