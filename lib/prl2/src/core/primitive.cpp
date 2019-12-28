#include "core/primitive.h"

namespace Prl2 {

Primitive::Primitive(const std::shared_ptr<Geometry>& _geometry,
                     const std::shared_ptr<Material>& _material,
                     const std::shared_ptr<Light>& _light)
    : geometry(_geometry), material(_material), light(_light) {}

bool Primitive::intersect(const Ray& ray, IntersectInfo& info) const {
  if (geometry->intersect(ray, info)) {
    // 衝突Primitiveをセット
    info.hitPrimitive = this;
    return true;
  } else {
    return false;
  }
}

}  // namespace Prl2