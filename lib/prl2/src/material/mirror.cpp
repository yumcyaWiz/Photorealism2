#include "material/mirror.h"

namespace Prl2 {

Mirror::Mirror(const SPD& _spd) : spd(_spd) {}

Real Mirror::sampleDirection(SurfaceInteraction& interaction, Sampler& sampler,
                             Real& pdf) const {
  interaction.wi_local = reflect(interaction.wo_local, Vec3(0, 1, 0));
  pdf = 1;
  return spd.sample(interaction.lambda) / absCosTheta(interaction.wi_local);
}

Real Mirror::BRDF(const SurfaceInteraction& interaction) const { return 0; }

RGB Mirror::albedoRGB(const SurfaceInteraction& interaction) const {
  return spd.toRGB();
}

}  // namespace Prl2