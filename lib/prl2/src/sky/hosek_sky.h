#ifndef _PRL2_HOSEK_SKY
#define _PRL2_HOSEK_SKY

#include "HosekWilkie/ArHosekSkyModel.h"

#include "core/spectrum.h"
#include "sky/sky.h"

namespace Prl2 {

class HosekSky : public Sky {
 public:
  HosekSky(const Real& albedo, const Real& turbidity,
           const Real& solarElevation) {
    for (int i = 0; i < SPD::LAMBDA_SAMPLES; ++i) {
      state[i] =
          arhosekskymodelstate_alloc_init(turbidity, albedo, solarElevation);
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
    const unsigned int index = (ray.lambda - SPD::LAMBDA_MIN) /
                               (SPD::LAMBDA_MAX - SPD::LAMBDA_MIN) *
                               SPD::LAMBDA_SAMPLES;
    return arhosekskymodel_radiance(state[index], theta, phi, ray.lambda);
  };

 private:
  ArHosekSkyModelState* state[SPD::LAMBDA_SAMPLES];
};

};  // namespace Prl2

#endif