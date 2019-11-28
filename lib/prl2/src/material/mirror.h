#ifndef _PRL2_MIRROR_H
#define _PRL2_MIRROR_H

#include "core/spectrum.h"
#include "material/material.h"

namespace Prl2 {

class Mirror : public Material {
 public:
  Mirror(const SPD& _albedo) : albedo(_albedo){};

  Real sampleDirection(const SurfaceInteraction& interaction, Sampler& sampler,
                       Vec3& wi_local, Real& pdf) const override {
    wi_local = reflect(interaction.wo_local, Vec3(0, 1, 0));
    pdf = 1;
    return albedo.sample(interaction.lambda) / absCosTheta(wi_local);
  };

 private:
  const SPD albedo;  // 分光反射率
};

}  // namespace Prl2

#endif