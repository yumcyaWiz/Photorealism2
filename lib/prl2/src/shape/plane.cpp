#include "shape/plane.h"

namespace Prl2 {

bool Plane::intersect(const Ray& ray, IntersectInfo& info) const {
  const Real t = -ray.origin.y() / ray.direction.y();
  if (std::isnan(t)) return false;

  const Vec3 hitPos = ray(t);
  if (std::abs(hitPos.x()) > 0.5 || std::abs(hitPos.z()) > 0.5) return false;

  info.t = t;
  info.hitPos = hitPos;
  info.hitNormal = Vec3(0, 1, 0);
  info.dpdu = Vec3(1, 0, 0);
  info.dpdv = Vec3(0, 0, -1);
  return true;
}

}  // namespace Prl2