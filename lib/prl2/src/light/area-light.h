#ifndef _PRL2_AREA_LIGHT_H
#define _PRL2_AREA_LIGHT_H

#include <memory>

#include "light/light.h"
#include "shape/shape.h"

namespace Prl2 {

// 形状を持つ光源を表現するクラス
class AreaLight : public Light {
 public:
  AreaLight(const SPD& _spd, const std::shared_ptr<Shape>& _shape);

  Real Le(const Ray& ray, const IntersectInfo& info) const override;

 private:
  std::shared_ptr<Shape> shape;  // 光源の形状
};

}  // namespace Prl2

#endif