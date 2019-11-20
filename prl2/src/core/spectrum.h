#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <iomanip>
#include <iostream>
#include <vector>

#include "core/type.h"
#include "core/vec3.h"

namespace Prl2 {

using XYZ = Vec3;
using RGB = Vec3;

//等間隔にサンプリングされたSPDを表現する
//波長と放射束のサンプリング列を保持する
//波長は[nm]で保持する
//波長の分割幅より狭いピークを持つSPDは適切に表現されない可能性がある
//本当はサンプルをそのまま保持して非等間隔のSPDを表現できるようにしたかったが、データサイズが大きすぎるので諦めた
class SPD {
 public:
  // SPDに格納する波長の範囲
  static constexpr Real LAMBDA_MIN = 380;
  static constexpr Real LAMBDA_MAX = 780;

  //波長の分割数
  static constexpr int LAMBDA_SAMPLES = 80;

  //分割された波長幅
  static constexpr Real LAMBDA_INTERVAL =
      (LAMBDA_MAX - LAMBDA_MIN) / LAMBDA_SAMPLES;

  Real phi[LAMBDA_SAMPLES];  //放射束

  // 0で初期化
  SPD() {
    for (int i = 0; i < LAMBDA_SAMPLES; ++i) {
      phi[i] = 0;
    }
  };

  // ある値で初期化
  SPD(const Real& v) {
    for (int i = 0; i < LAMBDA_SAMPLES; ++i) {
      phi[i] = v;
    }
  };

  //任意の波長と放射束のサンプリング列から等間隔のSPDを構築
  //波長と対応する放射束は昇順で並んでいると仮定している
  SPD(const std::vector<Real>& _lambda, const std::vector<Real>& _phi);

  //分光放射束を加算する
  void addPhi(const Real& _lambda, const Real& _phi);

  //黒色か返す
  bool isBlack() const {
    for (int i = 0; i < LAMBDA_SAMPLES; ++i) {
      if (phi[i] != 0.0f) {
        return false;
      }
    }
    return true;
  }

  //指定した波長の放射束を線形補間して返す
  // l : 波長[nm]
  Real sample(const Real& l) const;

  // XYZ色空間に変換する
  XYZ toXYZ() const;

  // sRGB色空間に変換する
  // XYZ to sRGB(D65)
  // http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
  RGB toRGB() const {
    XYZ xyz = this->toXYZ();
    return RGB(
        3.2404542f * xyz.x() - 1.5371385f * xyz.y() - 0.4985314f * xyz.z(),
        -0.9692660f * xyz.x() + 1.8760108f * xyz.y() + 0.0415560f * xyz.z(),
        0.0556434f * xyz.x() - 0.2040259f * xyz.y() + 1.0572252f * xyz.z());
  };

  //演算
  SPD& operator+=(const SPD& spd) {
    for (int i = 0; i < LAMBDA_SAMPLES; ++i) {
      phi[i] += spd.phi[i];
    }
    return *this;
  };
  SPD& operator+=(const Real& k) {
    for (int i = 0; i < LAMBDA_SAMPLES; ++i) {
      phi[i] += k;
    }
    return *this;
  };
  SPD& operator-=(const SPD& spd) {
    for (int i = 0; i < LAMBDA_SAMPLES; ++i) {
      phi[i] -= spd.phi[i];
    }
    return *this;
  };
  SPD& operator-=(const Real& k) {
    for (int i = 0; i < LAMBDA_SAMPLES; ++i) {
      phi[i] -= k;
    }
    return *this;
  };
  SPD& operator*=(const SPD& spd) {
    for (int i = 0; i < LAMBDA_SAMPLES; ++i) {
      phi[i] *= spd.phi[i];
    }
    return *this;
  };
  SPD& operator*=(const Real& k) {
    for (int i = 0; i < LAMBDA_SAMPLES; ++i) {
      phi[i] *= k;
    }
    return *this;
  };
  SPD& operator/=(const SPD& spd) {
    for (int i = 0; i < LAMBDA_SAMPLES; ++i) {
      phi[i] /= spd.phi[i];
    }
    return *this;
  };
  SPD& operator/=(const Real& k) {
    for (int i = 0; i < LAMBDA_SAMPLES; ++i) {
      phi[i] /= k;
    }
    return *this;
  };

 private:
  //等色関数(CIE1931)
  // http://cvrl.ucl.ac.uk/cmfs.htm
  static constexpr int color_matching_func_samples = 85;
  static constexpr Real color_matching_func_x[color_matching_func_samples] = {
      0.001368000000, 0.002236000000, 0.004243000000, 0.007650000000,
      0.014310000000, 0.023190000000, 0.043510000000, 0.077630000000,
      0.134380000000, 0.214770000000, 0.283900000000, 0.328500000000,
      0.348280000000, 0.348060000000, 0.336200000000, 0.318700000000,
      0.290800000000, 0.251100000000, 0.195360000000, 0.142100000000,
      0.095640000000, 0.057950010000, 0.032010000000, 0.014700000000,
      0.004900000000, 0.002400000000, 0.009300000000, 0.029100000000,
      0.063270000000, 0.109600000000, 0.165500000000, 0.225749900000,
      0.290400000000, 0.359700000000, 0.433449900000, 0.512050100000,
      0.594500000000, 0.678400000000, 0.762100000000, 0.842500000000,
      0.916300000000, 0.978600000000, 1.026300000000, 1.056700000000,
      1.062200000000, 1.045600000000, 1.002600000000, 0.938400000000,
      0.854449900000, 0.751400000000, 0.642400000000, 0.541900000000,
      0.447900000000, 0.360800000000, 0.283500000000, 0.218700000000,
      0.164900000000, 0.121200000000, 0.087400000000, 0.063600000000,
      0.046770000000, 0.032900000000, 0.022700000000, 0.015840000000,
      0.011359160000, 0.008110916000, 0.005790346000, 0.004109457000,
      0.002899327000, 0.002049190000, 0.001439971000, 0.000999949300,
      0.000690078600, 0.000476021300, 0.000332301100, 0.000234826100,
      0.000166150500, 0.000117413000, 0.000083075270, 0.000058706520,
      0.000041509940};
  static constexpr Real color_matching_func_y[color_matching_func_samples] = {
      0.000039000000, 0.000064000000, 0.000120000000, 0.000217000000,
      0.000396000000, 0.000640000000, 0.001210000000, 0.002180000000,
      0.004000000000, 0.007300000000, 0.011600000000, 0.016840000000,
      0.023000000000, 0.029800000000, 0.038000000000, 0.048000000000,
      0.060000000000, 0.073900000000, 0.090980000000, 0.112600000000,
      0.139020000000, 0.169300000000, 0.208020000000, 0.258600000000,
      0.323000000000, 0.407300000000, 0.503000000000, 0.608200000000,
      0.710000000000, 0.793200000000, 0.862000000000, 0.914850100000,
      0.954000000000, 0.980300000000, 0.994950100000, 1.000000000000,
      0.995000000000, 0.978600000000, 0.952000000000, 0.915400000000,
      0.870000000000, 0.816300000000, 0.757000000000, 0.694900000000,
      0.631000000000, 0.566800000000, 0.503000000000, 0.441200000000,
      0.381000000000, 0.321000000000, 0.265000000000, 0.217000000000,
      0.175000000000, 0.138200000000, 0.107000000000, 0.081600000000,
      0.061000000000, 0.044580000000, 0.032000000000, 0.023200000000,
      0.017000000000, 0.011920000000, 0.008210000000, 0.005723000000,
      0.004102000000, 0.002929000000, 0.002091000000, 0.001484000000,
      0.001047000000, 0.000740000000, 0.000520000000, 0.000361100000,
      0.000249200000, 0.000171900000, 0.000120000000, 0.000084800000,
      0.000060000000, 0.000042400000, 0.000030000000, 0.000021200000,
      0.000014990000};
  static constexpr Real color_matching_func_z[color_matching_func_samples] = {
      0.006450001000, 0.010549990000, 0.020050010000, 0.036210000000,
      0.067850010000, 0.110200000000, 0.207400000000, 0.371300000000,
      0.645600000000, 1.039050100000, 1.385600000000, 1.622960000000,
      1.747060000000, 1.782600000000, 1.772110000000, 1.744100000000,
      1.669200000000, 1.528100000000, 1.287640000000, 1.041900000000,
      0.812950100000, 0.616200000000, 0.465180000000, 0.353300000000,
      0.272000000000, 0.212300000000, 0.158200000000, 0.111700000000,
      0.078249990000, 0.057250010000, 0.042160000000, 0.029840000000,
      0.020300000000, 0.013400000000, 0.008749999000, 0.005749999000,
      0.003900000000, 0.002749999000, 0.002100000000, 0.001800000000,
      0.001650001000, 0.001400000000, 0.001100000000, 0.001000000000,
      0.000800000000, 0.000600000000, 0.000340000000, 0.000240000000,
      0.000190000000, 0.000100000000, 0.000049999990, 0.000030000000,
      0.000020000000, 0.000010000000, 0.000000000000, 0.000000000000,
      0.000000000000, 0.000000000000, 0.000000000000, 0.000000000000,
      0.000000000000, 0.000000000000, 0.000000000000, 0.000000000000,
      0.000000000000, 0.000000000000, 0.000000000000, 0.000000000000,
      0.000000000000, 0.000000000000, 0.000000000000, 0.000000000000,
      0.000000000000, 0.000000000000, 0.000000000000, 0.000000000000,
      0.000000000000, 0.000000000000, 0.000000000000, 0.000000000000,
      0.000000000000};
};

// SPDどうしの演算
//要素ごとに演算を行う
inline SPD operator+(const SPD& spd1, const SPD& spd2) {
  SPD ret;
  for (int i = 0; i < SPD::LAMBDA_SAMPLES; ++i) {
    ret.phi[i] = spd1.phi[i] + spd2.phi[i];
  }
  return ret;
}
inline SPD operator-(const SPD& spd1, const SPD& spd2) {
  SPD ret;
  for (int i = 0; i < SPD::LAMBDA_SAMPLES; ++i) {
    ret.phi[i] = spd1.phi[i] - spd2.phi[i];
  }
  return ret;
}
inline SPD operator*(const SPD& spd1, const SPD& spd2) {
  SPD ret;
  for (int i = 0; i < SPD::LAMBDA_SAMPLES; ++i) {
    ret.phi[i] = spd1.phi[i] * spd2.phi[i];
  }
  return ret;
}
inline SPD operator/(const SPD& spd1, const SPD& spd2) {
  SPD ret;
  for (int i = 0; i < SPD::LAMBDA_SAMPLES; ++i) {
    ret.phi[i] = spd1.phi[i] / spd2.phi[i];
  }
  return ret;
}

// SPDとRealの演算
inline SPD operator+(const SPD& spd, const Real& k) {
  SPD ret;
  for (int i = 0; i < SPD::LAMBDA_SAMPLES; ++i) {
    ret.phi[i] = spd.phi[i] + k;
  }
  return ret;
}
inline SPD operator+(const Real& k, const SPD& spd) { return spd + k; }
inline SPD operator-(const SPD& spd, const Real& k) {
  SPD ret;
  for (int i = 0; i < SPD::LAMBDA_SAMPLES; ++i) {
    ret.phi[i] = spd.phi[i] - k;
  }
  return ret;
}
inline SPD operator-(const Real& k, const SPD& spd) {
  SPD ret;
  for (int i = 0; i < SPD::LAMBDA_SAMPLES; ++i) {
    ret.phi[i] = k - spd.phi[i];
  }
  return ret;
}
inline SPD operator*(const SPD& spd, const Real& k) {
  SPD ret;
  for (int i = 0; i < SPD::LAMBDA_SAMPLES; ++i) {
    ret.phi[i] = spd.phi[i] * k;
  }
  return ret;
}
inline SPD operator*(const Real& k, const SPD& spd) { return spd * k; }
inline SPD operator/(const SPD& spd, const Real& k) {
  SPD ret;
  for (int i = 0; i < SPD::LAMBDA_SAMPLES; ++i) {
    ret.phi[i] = spd.phi[i] / k;
  }
  return ret;
}
inline SPD operator/(const Real& k, const SPD& spd) {
  SPD ret;
  for (int i = 0; i < SPD::LAMBDA_SAMPLES; ++i) {
    ret.phi[i] = k / spd.phi[i];
  }
  return ret;
}

// SPDの出力
inline std::ostream& operator<<(std::ostream& stream, const SPD& spd) {
  stream << std::setw(12) << "lambda" << std::setw(12) << "phi" << std::endl;
  for (int i = 0; i < SPD::LAMBDA_SAMPLES; ++i) {
    const Real lambda = SPD::LAMBDA_MIN + i * SPD::LAMBDA_INTERVAL;
    stream << std::setw(12) << lambda << std::setw(12) << spd.phi[i]
           << std::endl;
  }
  return stream;
}

}  // namespace Prl2

#endif