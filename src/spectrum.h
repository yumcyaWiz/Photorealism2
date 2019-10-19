#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <vector>
#include <algorithm>

#include "type.h"
#include "rgb.h"


//非等間隔にサンプリングされたSPDを表現する
//波長と放射束のサンプリング列を保持する
class SPD {
  public:
  //SPDに格納する波長の範囲
  constexpr static Real LAMBDA_MIN = 380;
  constexpr static Real LAMBDA_MAX = 780;

  std::vector<Real> lambda; //波長
  std::vector<Real> phi; //放射束

  SPD() : lambda({LAMBDA_MIN, LAMBDA_MAX}), phi({0, 0}) {};
  SPD(const std::vector<Real>& _lambda, const std::vector<Real>& _phi) : lambda(_lambda), phi(_phi) {};

  //サンプリング数を返す
  std::size_t nSamples() const {
    return lambda.size();
  };

  //指定した波長の放射束を線形補間して返す
  //l : 波長[nm]
  Real sample(const Real& l) const {
    if (l < LAMBDA_MIN || l > LAMBDA_MAX) {
      std::cerr << "lambda is out of bounds" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    else {
      std::size_t lambda2_idx = std::lower_bound(lambda.begin(), lambda.end(), l) - lambda.begin();
      std::size_t lambda1_idx = lambda2_idx - 1;

      const Real t = (l - lambda[lambda1_idx]) / (lambda[lambda2_idx] - lambda[lambda1_idx]);
      return (1 - t)*phi[lambda1_idx] + t*phi[lambda2_idx];
    }
  };


  //サンプルを追加する
  void addSample(const Real& _l, const Real& _phi) {
    std::size_t insert_idx = std::lower_bound(lambda.begin(), lambda.end(), _l) - lambda.begin();
    if (lambda[insert_idx] == _l) {
      phi[insert_idx] = _phi;
    }
    else {
      lambda.insert(lambda.begin() + insert_idx - 1, _l);
      phi.insert(phi.begin() + insert_idx - 1, _phi);
    }
  };


  //XYZ色空間に変換する
  XYZ toXYZ() const {
    //https://www.sciencedirect.com/topics/engineering/color-matching-function
    return XYZ();
  };


  //RGB色空間に変換する
  RGB toRGB() const {
    XYZ xyz = this->toXYZ();
    return RGB(
      3.240479f*xyz.x - 1.537150f*xyz.y - 0.498535f*xyz.z,
      -0.969256f*xyz.x + 1.875991f*xyz.y + 0.041556f*xyz.z,
      0.055648f*xyz.x - 0.204043f*xyz.y + 1.057311f*xyz.z
    );
  };
};


#endif