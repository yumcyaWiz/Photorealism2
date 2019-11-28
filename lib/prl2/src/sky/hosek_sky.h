#ifndef _PRL2_HOSEK_SKY
#define _PRL2_HOSEK_SKY

#include "core/spectrum.h"
#include "sky/sky.h"

extern "C" {
#include "ArHosekSkyModel.h"
}

namespace Prl2 {

class HosekSky : public Sky {
 public:
  HosekSky(const Vec3& _sunDirection, const Real& turbidity, const SPD& albedo);
  ~HosekSky();

  Real getRadiance(const Ray& ray) const override;

 private:
  ArHosekSkyModelState* state[SPD::LAMBDA_SAMPLES];
  const Vec3 sunDirection;  // 太陽の方向
};

}  // namespace Prl2

#endif