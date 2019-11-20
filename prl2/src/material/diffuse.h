#ifndef DIFFUSE_H
#define DIFFUSE_H

#include "core/constant.h"
#include "core/spectrum.h"
#include "material/material.h"
#include "sampler/sampling.h"

namespace Prl2 {

class Diffuse : public Material {
 public:
  Diffuse(const SPD& _albedo) : albedo(_albedo){};

  Real sampleDirection(const Ray& ray, Sampler& sampler, Ray& ray_out,
                       Real& pdf) const {
    // Cosine Weighted Hemisphere Sampling
    const Vec2 u = sampler.getNext2D();
    const Vec3 dir = sampleCosineHemisphere(u);
    pdf = INV_PI * absCosTheta(dir);

    // BRDFを計算
    return INV_PI * albedo.sample(ray.lambda);
  };

 private:
  SPD albedo;  //分光反射率
};

}  // namespace Prl2

#endif