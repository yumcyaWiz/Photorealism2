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
  HosekSky(const Vec3& _sunDirection, const Real& turbidity, const SPD& albedo)
      : sunDirection(normalize(_sunDirection)) {
    Real solarElevation, _tmp;
    cartesianToSpherical(sunDirection, solarElevation, _tmp);
    solarElevation = PI_DIV_2 - solarElevation;

    for (int i = 0; i < SPD::LAMBDA_SAMPLES; ++i) {
      state[i] =
          arhosekskymodelstate_alloc_init(solarElevation, turbidity, albedo[i]);
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

      // gammaが太陽の視野角/2以下なら太陽光を計算
      if (gamma < 0.251 / 180.0 * PI) {
        ret = arhosekskymodel_solar_radiance(state[index], theta, gamma,
                                             ray.lambda);
      } else {
        ret = arhosekskymodel_radiance(state[index], theta, gamma, ray.lambda);
      }
    }

    if (std::isnan(ret)) {
      ret = 0;
    }
    return ret;
  };

 private:
  ArHosekSkyModelState* state[SPD::LAMBDA_SAMPLES];
  const Vec3 sunDirection;  // 太陽の方向
};

}  // namespace Prl2

#endif