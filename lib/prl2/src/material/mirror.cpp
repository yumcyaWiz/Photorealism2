#include "material/mirror.h"

namespace Prl2 {

Mirror::Mirror(const SPD& _spd) : spd(_spd) {}

Real Mirror::sampleDirection(const SurfaceInteraction& interaction,
                             Sampler& sampler, Vec3& wi_local,
                             Real& pdf) const {
  wi_local = reflect(interaction.wo_local, Vec3(0, 1, 0));
  pdf = 1;
  return spd.sample(interaction.lambda) / absCosTheta(wi_local);
}

RGB Mirror::albedoRGB(const SurfaceInteraction& interaction) const {
  return spd.toRGB();
}
}  // namespace Prl2