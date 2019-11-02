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

XYZ SPD::toXYZ() const { return XYZ(); }