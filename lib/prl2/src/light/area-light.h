#ifndef _PRL2_AREA_LIGHT_H
#define _PRL2_AREA_LIGHT_H

#include <memory>

#include "core/geometry.h"
#include "light/light.h"

namespace Prl2 {

// Geometryを持つ光源を表現するクラス
class AreaLight : public Light {
 public:
  AreaLight(const SPD& _spd, const std::shared_ptr<Geometry>& _geometry);

  Real Le(const Ray& ray, const IntersectInfo& info) const override;

  void samplePoint(const IntersectInfo& info, Sampler& sampler, Vec3& p,
                   Real& pdf) const override;

 private:
  std::shared_ptr<Geometry> geometry;  // Geometry
};

}  // namespace Prl2

#endif