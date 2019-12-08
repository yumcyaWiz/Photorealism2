#ifndef _PRL2_MIRROR_H
#define _PRL2_MIRROR_H

#include "core/spectrum.h"
#include "material/material.h"

namespace Prl2 {

class Mirror : public Material {
 public:
  Mirror(const SPD& _spd);

  Real sampleDirection(const SurfaceInteraction& interaction, Sampler& sampler,
                       Vec3& wi_local, Real& pdf) const override;

  RGB albedoRGB(const SurfaceInteraction& interaction) const override;

 private:
  const SPD spd;  // 分光反射率
};

}  // namespace Prl2

#endif