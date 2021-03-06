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

  Real sampleDirection(MaterialArgs& interaction, Sampler& sampler,
                       Real& pdf) const override;

  Real BRDF(const MaterialArgs& interaction) const override;

  RGB albedoRGB(const MaterialArgs& interaction) const override;

 private:
  SPD spd;  //分光反射率
};

}  // namespace Prl2

#endif