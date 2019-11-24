#ifndef _PRL2_HOSEK_SKY
#define _PRL2_HOSEK_SKY

extern "C" {
#include "ArHosekSkyModel.h"
}

#include "core/spectrum.h"
#include "sky/sky.h"

namespace Prl2 {

class HosekSky : public Sky {
 public:
  HosekSky(const Real& solarElevation, const Real& turbidity,
           const Real& albedo) {
    for (int i = 0; i < SPD::LAMBDA_SAMPLES; ++i) {
      state[i] =
          arhosekskymodelstate_alloc_init(solarElevation, turbidity, albedo);
    }
  };
  ~HosekSky() {
    for (int i = 0; i < SPD::LAMBDA_SAMPLES; ++i) {
      arhosekskymodelstate_free(state[i]);
    }
  };

  Real getRadiance(const Ray& ray) const override {
    // Compute (theta, phi)
    Real theta, phi;
    cartesianToSpherical(ray.direction, theta, phi);

    // Compute State Index
    const unsigned int index =
        (ray.lambda - 320) / (720 - 320) * SPD::LAMBDA_SAMPLES;

    Real ret = 0;
    if (ray.lambda > 320 && ray.lambda < 720) {
      ret =
          arhosekskymodel_radiance(state[index], theta, phi, ray.lambda) +
          arhosekskymodel_solar_radiance(state[index], theta, phi, ray.lambda);
    }

    if (std::isnan(ret)) {
      ret = 0;
    }
    return ret;
  };

 private:
  ArHosekSkyModelState* state[SPD::LAMBDA_SAMPLES];
};

}  // namespace Prl2

#endif