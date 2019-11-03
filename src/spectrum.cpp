#include <cassert>

#include "spectrum.h"

SPD::SPD(const std::vector<Real>& lambda, const std::vector<Real>& phi) {}

Real SPD::sample(const Real& l) const {
  assert(l >= LAMBDA_MIN && l < LAMBDA_MAX);

  //対応する波長のインデックスを計算
  const std::size_t lambda_index = (l - LAMBDA_MIN) / LAMBDA_INTERVAL;

  //放射束を線形補間して計算
  if (lambda_index == 0 || lambda_index == LAMBDA_SAMPLES - 1) {
    return phi[lambda_index];
  } else {
    const Real lambda_nearest = LAMBDA_MIN + lambda_index * LAMBDA_INTERVAL;
    const Real t = (l - lambda_nearest) / LAMBDA_INTERVAL;
    assert(t >= 0 && t <= 1);
    return (1.0f - t) * phi[lambda_index] + t * phi[lambda_index + 1];
  }
}

//等色関数は線形補間して使用する
XYZ SPD::toXYZ() const {
  XYZ xyz;

  for (std::size_t i = 0; i < LAMBDA_SAMPLES; ++i) {
    const Real lambda_value = LAMBDA_MIN + LAMBDA_INTERVAL * i;
    const Real phi_value = phi[i];

    //放射束が0の場合は計算をスキップ
    if (phi_value == 0) continue;

    //対応する等色関数のインデックスを計算
    const unsigned int index = (lambda_value - 380) / 5;

    //等色関数を線形補間
    Real cmf_x, cmf_y, cmf_z;
    if (index != color_matching_func_samples - 1) {
      const unsigned int cmf_lambda = 5 * index + 380;
      const Real t = static_cast<Real>(lambda_value - cmf_lambda) / 5;
      cmf_x = (1.0f - t) * color_matching_func_x[index] +
              t * color_matching_func_x[index + 1];
      cmf_y = (1.0f - t) * color_matching_func_y[index] +
              t * color_matching_func_y[index + 1];
      cmf_z = (1.0f - t) * color_matching_func_z[index] +
              t * color_matching_func_z[index + 1];
    } else {
      cmf_x = color_matching_func_x[index];
      cmf_y = color_matching_func_y[index];
      cmf_z = color_matching_func_z[index];
    }

    // XYZを計算(短冊近似)
    xyz.x += cmf_x * phi_value;
    xyz.y += cmf_y * phi_value;
    xyz.z += cmf_z * phi_value;
  }

  return xyz;
}