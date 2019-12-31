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

bool Primitive::occluded(const Ray& ray, IntersectInfo& info) const {
  if (geometry->occluded(ray)) {
    // 衝突Primitiveをセット
    info.hitPrimitive = this;
    return true;
  } else {
    return false;
  }
}

Bounds3 Primitive::getBounds() const { return geometry->getBounds(); }

bool Primitive::isLight() const { return light != nullptr; }

const std::shared_ptr<Geometry>& Primitive::getGeometry() const {
  return geometry;
}

const std::shared_ptr<Material>& Primitive::getMaterial() const {
  return material;
}

const std::shared_ptr<Light>& Primitive::getLight() const { return light; }

unsigned int Primitive::getID() const { return id; }

void Primitive::setID(unsigned int _id) { id = _id; }

Real Primitive::sampleBRDF(const Vec3& wo, const Vec3& n, Real lambda,
                           Sampler& sampler, Vec3& wi, Real& cos,
                           Real& pdf) const {
  Vec3 s, t;
  orthonormalBasis(n, s, t);

  MaterialArgs args;
  args.lambda = lambda;
  args.wo_local = worldToMaterial(wo, s, n, t);

  const Real brdf = material->sampleDirection(args, sampler, pdf);
  cos = absCosTheta(args.wi_local);
  wi = materialToWorld(args.wi_local, s, n, t);

  return brdf;
}

Real Primitive::BRDF(const Vec3& wo, const Vec3& n, Real lambda,
                     const Vec3& wi) const {
  Vec3 s, t;
  orthonormalBasis(n, s, t);

  MaterialArgs args;
  args.lambda = lambda;
  args.wo_local = worldToMaterial(wo, s, n, t);
  args.wi_local = worldToMaterial(wi, s, n, t);

  return material->BRDF(args);
}

}  // namespace Prl2