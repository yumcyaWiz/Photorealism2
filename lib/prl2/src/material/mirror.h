#ifndef _PRL2_MIRROR_H
#define _PRL2_MIRROR_H

#include "core/spectrum.h"
#include "material/material.h"

namespace Prl2 {

class Mirror : public Material {
 public:
  Mirror(const SPD& _albedo) : albedo(_albedo){};

  Real sampleDirection(const Vec3& wo_local, const Real& lambda,
                       Sampler& sampler, Vec3& wi_local,
                       Real& pdf) const override {
    return 0;
  };

 private:
  const SPD albedo;  // 分光反射率
};

}  // namespace Prl2

#endif