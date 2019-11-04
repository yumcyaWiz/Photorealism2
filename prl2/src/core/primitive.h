#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <memory>

#include "core/isect.h"
#include "core/ray.h"
#include "core/transform.h"

#include "shape/shape.h"

namespace Prl2 {

class Primitive {
  std::shared_ptr<Shape> shape;  //形状
  std::shared_ptr<Transform>
      localToWorld;  //ローカル座標系からワールド座標系への変換を表すTransform

  Primitive(const std::shared_ptr<Shape>& _shape,
            const std::shared_ptr<Transform>& _localToWorld)
      : shape(_shape), localToWorld(_localToWorld){};

  //ワールド座標系のレイを受け取り、shapeとの衝突計算を行う。結果をinfoに保存する。
  bool intersect(const Ray& ray, IntersectInfo& info) const { return false; };
};

}  // namespace Prl2

#endif