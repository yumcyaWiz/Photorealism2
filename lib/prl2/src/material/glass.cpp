#include "material/glass.h"

namespace Prl2 {

Glass::Glass(const SellmeierEquation& _sellmeier, const SPD& _spd)
    : sellmeier(_sellmeier), spd(_spd) {}

Real Glass::sampleDirection(const SurfaceInteraction& interaction,
                            Sampler& sampler, Vec3& wi_local, Real& pdf) const {
  const bool is_entering = cosTheta(interaction.wo_local) > 0;
  const Real glass_ior = sellmeier.ior(interaction.lambda);

  const Vec3 normal = is_entering ? Vec3(0, 1, 0) : Vec3(0, -1, 0);
  const Real ior1 = is_entering ? 1.0 : glass_ior;
  const Real ior2 = is_entering ? glass_ior : 1.0;

  // Fresnel Cofficient
  const Real fr = fresnel(interaction.wo_local, normal, ior1, ior2);

  // Reflection
  if (sampler.getNext() < fr) {
    wi_local = reflect(interaction.wo_local, normal);
    pdf = fr;
    return fr * spd.sample(interaction.lambda) / absCosTheta(wi_local);
  }
  // Refract
  else {
    Vec3 wt;
    if (refract(interaction.wo_local, wt, normal, ior1, ior2)) {
      wi_local = wt;
      pdf = 1 - fr;
      return (1 - fr) * spd.sample(interaction.lambda) / absCosTheta(wi_local);
    }
    // Total Refrection
    else {
      wi_local = reflect(interaction.wo_local, normal);
      pdf = 1 - fr;
      return (1 - fr) * spd.sample(interaction.lambda) / absCosTheta(wi_local);
    }
  }
}

RGB Glass::albedoRGB(const SurfaceInteraction& interaction) const {
  return spd.toRGB();
}

}  // namespace Prl2