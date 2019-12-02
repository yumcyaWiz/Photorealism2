#ifndef DIFFUSE_H
#define DIFFUSE_H

#include "core/constant.h"
#include "core/spectrum.h"
#include "material/material.h"
#include "sampler/sampling.h"

namespace Prl2 {

class Diffuse : public Material {
 public:
  Diffuse(const SPD& _spd) : spd(_spd){};

  Real sampleDirection(const SurfaceInteraction& interaction, Sampler& sampler,
                       Vec3& wi_local, Real& pdf) const {
    // Cosine Weighted Hemisphere Sampling
    const Vec2 u = sampler.getNext2D();
    wi_local = sampleCosineHemisphere(u);
    pdf = INV_PI * absCosTheta(wi_local);

    // BRDFを計算
    return INV_PI * spd.sample(interaction.lambda);
  };

  Real albedo(const SurfaceInteraction& interaction) const {
    return spd.sample(interaction.lambda);
  };

 private:
  SPD spd;  //分光反射率
};

}  // namespace Prl2

#endif