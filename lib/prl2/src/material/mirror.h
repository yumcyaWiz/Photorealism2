#ifndef _PRL2_MIRROR_H
#define _PRL2_MIRROR_H

#include "core/spectrum.h"
#include "material/material.h"

namespace Prl2 {

class Mirror : public Material {
 public:
  Mirror(const SPD& _spd) : spd(_spd){};

  Real sampleDirection(const SurfaceInteraction& interaction, Sampler& sampler,
                       Vec3& wi_local, Real& pdf) const override {
    wi_local = reflect(interaction.wo_local, Vec3(0, 1, 0));
    pdf = 1;
    return spd.sample(interaction.lambda) / absCosTheta(wi_local);
  };

  RGB albedoRGB(const SurfaceInteraction& interaction) const {
    return spd.toRGB();
  };

 private:
  const SPD spd;  // 分光反射率
};

}  // namespace Prl2

#endif