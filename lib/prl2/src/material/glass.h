#ifndef _PRL2_GLASS_H
#define _PRL2_GLASS_H

#include "core/spectrum.h"
#include "material/material.h"

namespace Prl2 {

class Glass : public Material {
  Glass(const SPD& _albedo) : albedo(_albedo){};

  Real sampleDirection(const Vec3& wo_local, const Real& lambda,
                       Sampler& sampler, Vec3& wi_local,
                       Real& pdf) const override {
    return albedo.sample(lambda);
  };

 private:
  const SPD albedo;  // 分光反射率
};

}  // namespace Prl2

#endif