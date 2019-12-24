#ifndef DIFFUSE_H
#define DIFFUSE_H

#include "core/constant.h"
#include "core/spectrum.h"
#include "material/material.h"
#include "sampler/sampling.h"

namespace Prl2 {

class Diffuse : public Material {
 public:
  Diffuse(const SPD& _spd);

  Real sampleDirection(const SurfaceInteraction& interaction, Sampler& sampler,
                       Vec3& wi_local, Real& pdf) const override;

  Real BRDF(const SurfaceInteraction& interaction) const override;

  RGB albedoRGB(const SurfaceInteraction& interaction) const override;

 private:
  SPD spd;  //分光反射率
};

}  // namespace Prl2

#endif