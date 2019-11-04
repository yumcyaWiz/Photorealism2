#ifndef MAT4_H
#define MAT4_H

#include <iostream>

#include "type.h"
#include "vec4.h"

namespace Prl2 {

struct alignas(64) Mat4 {
  Real m[4][4];

  Mat4() {
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        m[i][j] = 0;
      }
    }
  };
  Mat4(const Real& _x) {
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        m[i][j] = _x;
      }
    }
  }
  Mat4(const Real _m[4][4]) {
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        m[i][j] = _m[i][j];
      }
    }
  }
  Mat4(const Real& m00, const Real& m01, const Real& m02, const Real& m03,
       const Real& m10, const Real& m11, const Real& m12, const Real& m13,
       const Real& m20, const Real& m21, const Real& m22, const Real& m23,
       const Real& m30, const Real& m31, const Real& m32, const Real& m33) {
    m[0][0] = m00;
    m[0][1] = m01;
    m[0][2] = m02;
    m[0][3] = m03;
    m[1][0] = m10;
    m[1][1] = m11;
    m[1][2] = m12;
    m[1][3] = m13;
    m[2][0] = m20;
    m[2][1] = m21;
    m[2][2] = m22;
    m[2][3] = m23;
    m[3][0] = m30;
    m[3][1] = m31;
    m[3][2] = m32;
    m[3][3] = m33;
  };

  Vec4 operator[](std::size_t i) const {
    if (i >= 4) {
      std::cerr << "invalid mat4 index" << std::endl;
      std::exit(EXIT_FAILURE);
    } else {
      return Vec4(m[i][0], m[i][1], m[i][2], m[i][3]);
    }
  };
};

inline Mat4 operator+(const Mat4& m1, const Mat4& m2) {
  Mat4 ret;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      ret.m[i][j] = m1.m[i][j] + m2.m[i][j];
    }
  }
  return ret;
}

inline Mat4 operator-(const Mat4& m1, const Mat4& m2) {
  Mat4 ret;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      ret.m[i][j] = m1.m[i][j] - m2.m[i][j];
    }
  }
  return ret;
}

inline Mat4 operator*(const Mat4& m1, const Mat4& m2) {
  Mat4 ret;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      for (int k = 0; k < 4; ++k) {
        ret.m[i][j] += m1.m[i][k] * m2.m[k][j];
      }
    }
  }
  return ret;
}

inline Vec4 operator*(const Mat4& m, const Vec4& v) {
  Vec4 ret;
  for (int i = 0; i < 4; ++i) {
    ret[i] = m.m[i][0] * v.x() + m.m[i][1] * v.y() + m.m[i][2] * v.z() +
             m.m[i][3] * v.w();
  }
  return ret;
}

inline Mat4 identity() {
  return Mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
}

inline Mat4 transpose(const Mat4& m) {
  return Mat4(m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0], m.m[0][1], m.m[1][1],
              m.m[2][1], m.m[3][1], m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2],
              m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3]);
}

std::ostream& operator<<(std::ostream& stream, const Mat4& m) {
  stream << "(";
  for (int i = 0; i < 4; ++i) {
    if (i != 3) {
      stream << m[i] << "," << std::endl;
    } else {
      stream << m[i];
    }
  }
  stream << ")";
  return stream;
}

}  // namespace Prl2

#endif