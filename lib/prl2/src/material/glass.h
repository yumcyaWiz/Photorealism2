#ifndef _PRL2_GLASS_H
#define _PRL2_GLASS_H

#include "core/spectrum.h"
#include "material/material.h"

namespace Prl2 {

// Sellmeierの式を表す
struct SellmeierEquation {
  SellmeierEquation(const Real& _b1, const Real& _b2, const Real& _b3,
                    const Real& _c1, const Real& _c2, const Real& _c3)
      : b1(_b1), b2(_b2), b3(_b3), c1(_c1), c2(_c2), c3(_c3){};

  // 屈折率を計算する
  // lambdaの単位は[nm]
  Real ior(const Real& lambda) const {
    const Real l = lambda * 1e-3;
    return std::sqrt(1 + b1 * l * l / (l * l - c1) + b2 * l * l / (l * l - c2) +
                     b3 * l * l / (l * l - c3));
  };

  // Sellmeierの式の係数
  // 単位は[μm]
  const Real b1;
  const Real b2;
  const Real b3;
  const Real c1;
  const Real c2;
  const Real c3;
};

class Glass : public Material {
 public:
  Glass(const SellmeierEquation& _sellmeier, const SPD& _albedo)
      : sellmeier(_sellmeier), albedo(_albedo){};

  Real sampleDirection(const SurfaceInteraction& interaction, Sampler& sampler,
                       Vec3& wi_local, Real& pdf) const override {
    const Real glass_ior = sellmeier.ior(interaction.lambda);
    const Real fr = fresnel(interaction.wo_local, Vec3(0, 1, 0),
                            interaction.ior, glass_ior);

    // Reflect
    if (sampler.getNext() < fr) {
      wi_local = reflect(interaction.wo_local, Vec3(0, 1, 0));
      pdf = fr;
      return albedo.sample(interaction.lambda) / absCosTheta(wi_local);
    }
    // Refract
    else {
    }
  };

 private:
  const SellmeierEquation sellmeier;  //セルマイヤーの式
  const SPD albedo;                   // 分光反射率
};

}  // namespace Prl2

#endif