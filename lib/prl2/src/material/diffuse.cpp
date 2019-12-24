#include "material/diffuse.h"

namespace Prl2 {

Diffuse::Diffuse(const SPD& _spd) : spd(_spd) {}

Real Diffuse::sampleDirection(const SurfaceInteraction& interaction,
                              Sampler& sampler, Vec3& wi_local,
                              Real& pdf) const {
  // Cosine Weighted Hemisphere Sampling
  const Vec2 u = sampler.getNext2D();
  wi_local = sampleCosineHemisphere(u);
  pdf = INV_PI * absCosTheta(wi_local);

  // BRDFを計算
  return INV_PI * spd.sample(interaction.lambda);
}

Real Diffuse::BRDF(const SurfaceInteraction& interaction) const { return 0; }

RGB Diffuse::albedoRGB(const SurfaceInteraction& interaction) const {
  return spd.toRGB();
}

}  // namespace Prl2