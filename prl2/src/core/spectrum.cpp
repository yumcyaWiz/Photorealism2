#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>

#include "core/spectrum.h"

namespace Prl2 {

//非等間隔なSPDから等間隔なSPDを構成する
//非等間隔なSPDを線形補間して計算する
SPD::SPD(const std::vector<Real>& _lambda, const std::vector<Real>& _phi) {
  for (int i = 0; i < LAMBDA_SAMPLES; ++i) {
    //等間隔側の波長
    const Real lambda_value = LAMBDA_MIN + LAMBDA_INTERVAL * i;

    //指定した波長が非等間隔のSPDの範囲に含まれていない場合、放射束を0とする
    if (lambda_value < _lambda.front() || lambda_value > _lambda.back()) {
      phi[i] = 0;
    } else if (lambda_value == _lambda.front()) {
      phi[i] = _phi.front();
    } else if (lambda_value == _lambda.back()) {
      phi[i] = _phi.back();
    } else {
      //非等間隔のSPDを線形補間
      const int lambda1_index =
          std::lower_bound(_lambda.begin(), _lambda.end(), lambda_value) -
          _lambda.begin();
      const int lambda0_index = lambda1_index - 1;
      assert(lambda0_index != lambda1_index);

      const Real t = (lambda_value - _lambda[lambda0_index]) /
                     (_lambda[lambda1_index] - _lambda[lambda0_index]);
      assert(t >= 0 && t <= 1);

      const Real interpolated_phi =
          (1.0f - t) * _phi[lambda0_index] + t * _phi[lambda1_index];
      phi[i] = interpolated_phi;
    }
  }
}

void SPD::addPhi(const Real& _lambda, const Real& _phi) {
  assert(_lambda >= LAMBDA_MIN && _lambda <= LAMBDA_MAX);

  //対応する波長のインデックスを計算
  const int lambda_index = (_lambda - LAMBDA_MIN) / LAMBDA_INTERVAL;

  //両側に寄与を分配する
  const Real lambda0 =
      LAMBDA_MIN + lambda_index * LAMBDA_INTERVAL;  //左側の波長
  const Real t =
      (_lambda - lambda0) / LAMBDA_INTERVAL;  //与えられた波長の区間中の位置
  phi[lambda_index] += (1.0f - t) * _phi;
  phi[lambda_index + 1] += t * _phi;
}

Real SPD::sample(const Real& l) const {
  assert(l >= LAMBDA_MIN && l < LAMBDA_MAX);

  //対応する波長のインデックスを計算
  const int lambda_index = (l - LAMBDA_MIN) / LAMBDA_INTERVAL;

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
    const int index = (lambda_value - 380) / 5;
    assert(index >= 0 && index < color_matching_func_samples);

    //等色関数を線形補間
    Real cmf_x, cmf_y, cmf_z;
    if (index != color_matching_func_samples - 1) {
      const Real cmf_lambda = 5 * index + 380;
      const Real t = (lambda_value - cmf_lambda) / 5;
      assert(t >= 0 && t <= 1);

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
    xyz[0] += cmf_x * phi_value;
    xyz[1] += cmf_y * phi_value;
    xyz[2] += cmf_z * phi_value;
  }

  return xyz;
}

}  // namespace Prl2