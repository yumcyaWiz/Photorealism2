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

  Real sampleDirection(SurfaceInteraction& interaction, Sampler& sampler,
                       Real& pdf) const {
    // Cosine Weighted Hemisphere Sampling
    const Vec2 u = sampler.getNext2D();
    const Vec3 wi = sampleCosineHemisphere(u);
    interaction.wi = wi;
    pdf = INV_PI * absCosTheta(wi);

    // BRDFを計算
    return INV_PI * albedo.sample(interaction.lambda);
  };

 private:
  SPD albedo;  //分光反射率
};

}  // namespace Prl2

#endif