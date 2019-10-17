#ifndef MATRIX4_H
#define MATRIX4_H

#include "type.h"

struct alignas(64) Matrix4 {
  Real m[4][4];

  Matrix4() {
    for(int i = 0; i < 4; ++i) {
      for(int j = 0; j < 4; ++j) {
        m[i][j] = 0;
      }
    }
  };
  Matrix4(const Real _m[4][4]) {
    for(int i = 0; i < 4; ++i) {
      for(int j = 0; j < 4; ++j) {
        m[i][j] = _m[i][j];
      }
    }
  }
  Matrix4(const Real& m00, const Real& m01, const Real& m02, const Real& m03, 
            const Real& m10, const Real& m11, const Real& m12, const Real& m13,
            const Real& m20, const Real& m21, const Real& m22, const Real& m23,
            const Real& m30, const Real& m31, const Real& m32, const Real& m33) {
    m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
    m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
    m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
    m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
  };
};


inline Matrix4 identity() {
  return Matrix4(1, 0, 0, 0,
                 0, 1, 0, 0,
                 0, 0, 1, 0,
                 0, 0, 0, 1);
}


inline Matrix4 transpose(const Matrix4& m) {
  return Matrix4(m[0][0], m[1][0], m[2][0], m[3][0],
                 m[0][1], m[1][1], m[2][1], m[3][1],
                 m[0][2], m[1][2], m[2][2], m[3][2],
                 m[0][3], m[1][3], m[2][3], m[3][3]);
}
#endif