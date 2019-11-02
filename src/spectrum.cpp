#include "spectrum.h"

Real SPD::sample(const Real& l) const {
  if (l < LAMBDA_MIN || l > LAMBDA_MAX) {
    std::cerr << "lambda is out of bounds" << std::endl;
    std::exit(EXIT_FAILURE);
  } else {
    std::size_t lambda2_idx =
        std::lower_bound(lambda.begin(), lambda.end(), l) - lambda.begin();
    std::size_t lambda1_idx = lambda2_idx - 1;

    const Real t =
        (l - lambda[lambda1_idx]) / (lambda[lambda2_idx] - lambda[lambda1_idx]);
    return (1 - t) * phi[lambda1_idx] + t * phi[lambda2_idx];
  }
}

void SPD::addSample(const Real& _l, const Real& _phi) {
  std::size_t insert_idx =
      std::lower_bound(lambda.begin(), lambda.end(), _l) - lambda.begin();
  if (lambda[insert_idx] == _l) {
    phi[insert_idx] = _phi;
  } else {
    lambda.insert(lambda.begin() + insert_idx - 1, _l);
    phi.insert(phi.begin() + insert_idx - 1, _phi);
  }
}

//等色関数は線形補間して使用する
XYZ SPD::toXYZ() const {
  XYZ xyz;

  for (std::size_t i = 0; i < lambda.size(); ++i) {
    const Real lambda_value = lambda[i];
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

    // XYZを計算
    xyz.x += cmf_x * phi_value;
    xyz.y += cmf_y * phi_value;
    xyz.z += cmf_z * phi_value;
  }

  return xyz;
}