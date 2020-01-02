#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <memory>

#include "core/bounds3.h"
#include "core/geometry.h"
#include "core/isect.h"
#include "core/ray.h"
#include "light/light.h"
#include "material/material.h"

namespace Prl2 {

class Primitive {
 public:
  Primitive(const std::shared_ptr<Geometry>& _geometry,
            const std::shared_ptr<Material>& _material,
            const std::shared_ptr<Light>& _light = nullptr);

  // ワールド座標系のレイを受け取り、Geometryとの衝突計算を行う。結果をinfoに保存する。
  bool intersect(const Ray& ray, IntersectInfo& info) const;

  // ワールド座標系のレイを受け取り、Geometryとの衝突判定を行う
  // 衝突Primitiveのみがセットされる
  bool occluded(const Ray& ray, IntersectInfo& info) const;

  // ワールド座標系のバウンディングボックスを取得する
  Bounds3 getBounds() const;

  // 光源かどうか
  bool isLight() const;

  // BRDFから方向をサンプリングする
  Real sampleBRDF(const Vec3& wo, const Vec3& n, Real lambda, Sampler& sampler,
                  Vec3& wi, Real& cos, Real& pdf) const;

  // BRDFを計算する
  Real BRDF(const Vec3& wo, const Vec3& n, Real lambda, const Vec3& wi) const;

  const std::shared_ptr<Geometry>& getGeometry() const;
  const std::shared_ptr<Material>& getMaterial() const;
  const std::shared_ptr<Light>& getLight() const;

  // Primitive IDを取得する (for embree intersector)
  unsigned int getID() const;
  // Primitive IDをセットする (for embree intersector)
  void setID(unsigned int _id);

 private:
  const std::shared_ptr<Geometry> geometry;  // Geometry
  const std::shared_ptr<Material> material;  // Material
  const std::shared_ptr<Light> light;        // Light

  unsigned int id;  // (for embree intersector)
};

}  // namespace Prl2

#endif