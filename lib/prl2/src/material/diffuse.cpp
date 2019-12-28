#include "material/diffuse.h"

namespace Prl2 {

Diffuse::Diffuse(const SPD& _spd) : spd(_spd) {}

Real Diffuse::sampleDirection(MaterialArgs& interaction, Sampler& sampler,
                              Real& pdf) const {
  // Cosine Weighted Hemisphere Sampling
  const Vec2 u = sampler.getNext2D();
  interaction.wi_local = sampleCosineHemisphere(u);
  pdf = INV_PI * absCosTheta(interaction.wi_local);

  // BRDFを計算
  return INV_PI * spd.sample(interaction.lambda);
}

Real Diffuse::BRDF(const MaterialArgs& interaction) const {
  return INV_PI * spd.sample(interaction.lambda);
}

RGB Diffuse::albedoRGB(const MaterialArgs& interaction) const {
  return spd.toRGB();
}

}  // namespace Prl2