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
  HosekSky(const Real& turbidity, const Real& albedo) {
    sunDirection = normalize(Vec3(0, 0, 1));
    Real solarElevation, _tmp;
    cartesianToSpherical(sunDirection, solarElevation, _tmp);
    solarElevation = PI_DIV_2 - solarElevation;

    SPD ad = RGB2Spectrum(RGB(1));
    for (int i = 0; i < SPD::LAMBDA_SAMPLES; ++i) {
      state[i] =
          arhosekskymodelstate_alloc_init(solarElevation, turbidity, ad.phi[i]);
    }
  };
  ~HosekSky() {
    for (int i = 0; i < SPD::LAMBDA_SAMPLES; ++i) {
      arhosekskymodelstate_free(state[i]);
    }
  };

  Real getRadiance(const Ray& ray) const override {
    // Compute theta, gamma
    Real theta, phi;
    cartesianToSpherical(ray.direction, theta, phi);
    if (theta > PI_DIV_2) return 0;

    const Real gamma = radianBetween(ray.direction, sunDirection);

    Real ret = 0;
    if (ray.lambda >= 320 && ray.lambda < 720) {
      // Compute State Index
      const unsigned int index = (ray.lambda - SPD::LAMBDA_MIN) /
                                 (SPD::LAMBDA_MAX - SPD::LAMBDA_MIN) *
                                 SPD::LAMBDA_SAMPLES;

      ret = arhosekskymodel_radiance(state[index], theta, gamma, ray.lambda);
    }

    if (std::isnan(ret)) {
      ret = 0;
    }
    return ret;
  };

 private:
  ArHosekSkyModelState* state[SPD::LAMBDA_SAMPLES];
  Vec3 sunDirection;  // 太陽の方向
};

}  // namespace Prl2

#endif