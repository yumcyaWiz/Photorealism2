#ifndef _PRL2_GEOMETRY_H
#define _PRL2_GEOMETRY_H

#include <memory>

#include "core/isect.h"
#include "core/ray.h"
#include "core/transform.h"
#include "shape/shape.h"

namespace Prl2 {

// ShapeとTransformをセットで表現するクラス
class Geometry {
 public:
  Geometry(const std::shared_ptr<Shape>& _shape,
           const std::shared_ptr<Transform>& _localToWorld)
      : shape(_shape), localToWorld(_localToWorld) {
    // TODO: 面積測度のPDFの変換にはそのまま使用できないかも？
    area_scale = localToWorld->mat.m[0][0] * localToWorld->mat.m[1][1] *
                 localToWorld->mat.m[2][2];
  };

  bool intersect(const Ray& ray, IntersectInfo& info) const {
    //レイをローカル座標系に変換
    const Ray ray_local = localToWorld->applyInverse(ray);

    // shapeとの衝突計算
    IntersectInfo info_local;
    if (shape->intersect(ray_local, info_local)) {
      // ローカル座標系の衝突情報をワールド座標系に変換
      info = localToWorld->apply(info_local);
      return true;
    } else {
      return false;
    }
  };

 private:
  const std::shared_ptr<Shape> shape;  // Shape
  const std::shared_ptr<Transform>
      localToWorld;  // ローカル座標系からワールド座標系へのTransform
  Real area_scale;  // Transformによる表面積拡大率
};

}  // namespace Prl2

#endif