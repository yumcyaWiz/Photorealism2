#ifndef PRL2_GEOMETRY_H
#define PRL2_GEOMETRY_H

#include <memory>

#include "core/bounds3.h"
#include "core/isect.h"
#include "core/ray.h"
#include "core/transform.h"
#include "sampler/sampling.h"
#include "shape/shape.h"

namespace Prl2 {

// ShapeとTransformをセットで表現するクラス
class Geometry {
 public:
  Geometry(const std::shared_ptr<Shape>& _shape,
           const std::shared_ptr<Transform>& _localToWorld)
      : shape(_shape), localToWorld(_localToWorld){};

  // ワールド座標系のレイを受け取り、衝突計算を行う
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

  // ワールド座標系のレイを受け取り、衝突判定を行う
  bool occluded(const Ray& ray) const {
    //レイをローカル座標系に変換
    const Ray ray_local = localToWorld->applyInverse(ray);

    return shape->occluded(ray_local);
  }

  // Geometry上の点をサンプリングする
  void samplePoint(Sampler& sampler, Vec3& p, Vec3& n, Real& pdf_area) const {
    // ローカル座標系でサンプリング
    Vec3 p_local, n_local;
    shape->samplePoint(sampler, p_local, n_local, pdf_area);

    // ワールド座標系に変換
    p = localToWorld->applyPoint(p_local);
    n = localToWorld->applyNormal(n_local);
  };

  const std::shared_ptr<Shape>& getShape() const { return shape; };

  // ワールド座標系のバウンディングボックスを計算する
  Bounds3 getBounds() const { return localToWorld->apply(shape->getBounds()); };

 private:
  const std::shared_ptr<Shape> shape;  // Shape
  const std::shared_ptr<Transform>
      localToWorld;  // ローカル座標系からワールド座標系へのTransform
};

}  // namespace Prl2

#endif