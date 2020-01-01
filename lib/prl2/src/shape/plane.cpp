#include "shape/plane.h"

namespace Prl2 {

bool Plane::intersect(const Ray& ray, IntersectInfo& info) const {
  const Real t = -ray.origin[1] / ray.direction[1];
  if (t < ray.tmin || t > ray.tmax || std::isnan(t)) return false;

  const Vec3 hitPos = ray(t);
  if (std::abs(hitPos.x()) > 0.5 || std::abs(hitPos.z()) > 0.5) return false;

  info.t = t;
  info.hitPos = hitPos;
  info.hitNormal = Vec3(0, 1, 0);
  info.uv = Vec2(hitPos.x() + 0.5f, hitPos.z() + 0.5f);
  return true;
}

bool Plane::occluded(const Ray& ray) const {
  const Real t = -ray.origin[1] / ray.direction[1];
  if (t < ray.tmin || t > ray.tmax || std::isnan(t)) return false;

  const Vec3 hitPos = ray(t);
  if (std::abs(hitPos.x()) > 0.5 || std::abs(hitPos.z()) > 0.5) return false;

  return true;
}

void Plane::samplePoint(Sampler& sampler, Vec3& p, Vec3& n,
                        Real& pdf_area) const {
  const Vec2 u = sampler.getNext2D();
  p[0] = u.x() - 0.5f;
  p[1] = 0;
  p[2] = u.y() - 0.5f;
  n = Vec3(0, 1, 0);
  pdf_area = 1;
}

Bounds3 Plane::getBounds() const {
  return Bounds3(Vec3(-0.5, -EPS, -0.5), Vec3(0.5, EPS, 0.5));
}

}  // namespace Prl2