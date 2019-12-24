#include "material/diffuse.h"

namespace Prl2 {

Diffuse::Diffuse(const SPD& _spd) : spd(_spd) {}

Real Diffuse::sampleDirection(SurfaceInteraction& interaction, Sampler& sampler,
                              Real& pdf) const {
  // Cosine Weighted Hemisphere Sampling
  const Vec2 u = sampler.getNext2D();
  interaction.wi_local = sampleCosineHemisphere(u);
  pdf = INV_PI * absCosTheta(interaction.wi_local);

  // BRDFを計算
  return INV_PI * spd.sample(interaction.lambda);
}

Real Diffuse::BRDF(const SurfaceInteraction& interaction) const { return 0; }

RGB Diffuse::albedoRGB(const SurfaceInteraction& interaction) const {
  return spd.toRGB();
}

}  // namespace Prl2