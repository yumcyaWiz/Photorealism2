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

void AreaLight::samplePoint(const IntersectInfo& info, Sampler& sampler,
                            Vec3& p, Real& pdf) const {
  // 点をサンプリング
  Vec3 n;
  Real pdf_area;
  geometry->samplePoint(sampler, p, n, pdf_area);

  // 面積に関するpdfを立体角に関するpdfに変換
  const Real r_2 = length2(p - info.hitPos);
  const Real cos = std::abs(dot(normalize(info.hitPos - p), n));
  pdf = r_2 / cos * pdf_area;
}

}  // namespace Prl2
