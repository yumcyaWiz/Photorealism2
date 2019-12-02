#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <memory>

#include "core/isect.h"
#include "core/ray.h"

#include "core/geometry.h"
#include "light/light.h"
#include "material/material.h"

namespace Prl2 {

class Primitive {
 public:
  Primitive(const std::shared_ptr<Geometry>& _geometry,
            const std::shared_ptr<Material>& _material,
            const std::shared_ptr<Light>& _light = nullptr)
      : geometry(_geometry), material(_material), light(_light) {};

  // ワールド座標系のレイを受け取り、Geometryとの衝突計算を行う。結果をinfoに保存する。
  bool intersect(const Ray& ray, IntersectInfo& info) const {
    if (geometry->intersect(ray, info)) {
      // 衝突Primitiveをセット
      info.hitPrimitive = this;
      return true;
    } else {
      return false;
    }
  };

  const std::shared_ptr<Geometry> geometry;  // Geometry
  const std::shared_ptr<Material> material;  // Material
  const std::shared_ptr<Light> light;        // Light
};

}  // namespace Prl2

#endif