#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#include "type.h"
#include "vec3.h"

using XYZ = Vec3;
using RGB = Vec3;

//非等間隔にサンプリングされたSPDを表現する
//波長と放射束のサンプリング列を保持する
class SPD {
 public:
  // SPDに格納する波長の範囲
  constexpr static Real LAMBDA_MIN = 380;
  constexpr static Real LAMBDA_MAX = 780;

  std::vector<Real> lambda;  //波長
  std::vector<Real> phi;     //放射束

  SPD() : lambda({LAMBDA_MIN, LAMBDA_MAX}), phi({0, 0}){};
  SPD(const std::vector<Real>& _lambda, const std::vector<Real>& _phi)
      : lambda(_lambda), phi(_phi){};

  //サンプリング数を返す
  std::size_t nSamples() const { return lambda.size(); };

  //指定した波長の放射束を線形補間して返す
  // l : 波長[nm]
  Real sample(const Real& l) const;

  //サンプルを追加する
  void addSample(const Real& _l, const Real& _phi);

  // XYZ色空間に変換する
  // http://cvrl.ucl.ac.uk/cmfs.htm
  XYZ toXYZ() const;

  // sRGB色空間に変換する
  // XYZ to sRGB(D65)
  // http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
  RGB toRGB() const {
    XYZ xyz = this->toXYZ();
    return RGB(3.2404542f * xyz.x - 1.5371385f * xyz.y - 0.4985314f * xyz.z,
               -0.9692660f * xyz.x + 1.8760108f * xyz.y + 0.0415560f * xyz.z,
               0.0556434f * xyz.x - 0.2040259f * xyz.y + 1.0572252f * xyz.z);
  };
};

#endif