#include <cassert>

#include "spectrum.h"

//非等間隔なSPDから等間隔なSPDを構成する
//非等間隔なSPDを線形補間して計算する
SPD::SPD(const std::vector<Real>& _lambda, const std::vector<Real>& _phi) {
  for (int i = 0; i < LAMBDA_SAMPLES; ++i) {
    //等間隔側の波長
    const Real lambda_value = LAMBDA_MIN + LAMBDA_INTERVAL * i;

    //指定した波長が非等間隔のSPDの範囲に含まれていない場合、放射束を0とする
    if (lambda_value < _lambda.front() || lambda_value > _lambda.back()) {
      phi[i] = 0;
    } else {
      //非等間隔のSPDを線形補間
      const std::size_t lambda0_index =
          std::lower_bound(_lambda.begin(), _lambda.end(), lambda_value) -
          _lambda.begin() - 1;
      const std::size_t lambda1_index =
          std::upper_bound(_lambda.begin(), _lambda.end(), lambda_value) -
          _lambda.begin();
      assert(lambda0_index != lambda1_index);

      const Real t = (lambda_value - _lambda[lambda0_index]) /
                     (_lambda[lambda1_index] - _lambda[lambda0_index]);
      assert(t >= 0 && t <= 1);

      phi[i] = (1.0f - t) * _phi[lambda0_index] + t * _phi[lambda1_index];
    }
  }
}

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