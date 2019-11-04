#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "core/mat4.h"
#include "core/vec3.h"

namespace Prl2 {

//アフィン変換を行うクラス
class Transform {
 public:
  Mat4 mat;     //変換行列
  Mat4 invmat;  //逆変換行列

  Transform() : mat(identity()), invmat(identity()){};
  Transform(const Mat4& _mat, const Mat4& _invmat)
      : mat(_mat), invmat(_invmat){};

  Transform operator*(const Transform& t) const {
    return Transform(mat * t.mat, invmat * t.invmat);
  };

  //方向ベクトルに対して変換を施す
  Vec3 applyDirection(const Vec3& v) const {
    return Vec3(
        mat.m[0][0] + v.x() + mat.m[0][1] * v.y() + mat.m[0][2] * v.z(),
        mat.m[1][0] * v.x() + mat.m[1][1] * v.y() + mat.m[1][2] * v.z(),
        mat.m[2][0] * v.x() + mat.m[2][1] * v.y() + mat.m[2][2] * v.z());
  };

  //点ベクトルに対して変換を施す
  Vec3 applyPoint(const Vec3& v) const {
    const Real x = mat.m[0][0] * v.x() + mat.m[0][1] * v.y() +
                   mat.m[0][2] * v.z() + mat.m[0][3];
    const Real y = mat.m[1][0] * v.x() + mat.m[1][1] * v.y() +
                   mat.m[1][2] * v.z() + mat.m[1][3];
    const Real z = mat.m[2][0] * v.x() + mat.m[2][1] * v.y() +
                   mat.m[2][2] * v.z() + mat.m[2][3];
    const Real w = mat.m[3][0] * v.x() + mat.m[3][1] * v.y() +
                   mat.m[3][2] * v.z() + mat.m[3][3];
    return Vec3(x / w, y / w, z / w);
  };

  //法線ベクトルに対して変換を施す
  Vec3 applyNormal(const Vec3& n) const {
    return Vec3(invmat.m[0][0] * n.x() + invmat.m[1][0] * n.y() +
                    invmat.m[2][0] * n.z(),
                invmat.m[0][1] * n.x() + invmat.m[1][1] * n.y() +
                    invmat.m[2][1] * n.z(),
                invmat.m[0][2] * n.x() + invmat.m[1][1] * n.y() +
                    invmat.m[2][2] * n.z());
  };
};

//移動変換を表す行列を返す
Transform translate(const Vec3& v) {
  return Transform(
      Mat4(1, 0, 0, v.x(), 0, 1, 0, v.y(), 0, 0, 1, v.z(), 0, 0, 0, 1),
      Mat4(1, 0, 0, -v.x(), 0, 1, 0, -v.y(), 0, 0, 1, -v.z(), 0, 0, 0, 1));
}

//スケール変換を表す行列を返す
Transform scale(const Vec3& s) {
  return Transform(
      Mat4(s.x(), 0, 0, 0, 0, s.y(), 0, 0, 0, 0, s.z(), 0, 0, 0, 0, 1),
      Mat4(1 / s.x(), 0, 0, 0, 0, 1 / s.y(), 0, 0, 0, 0, 1 / s.z(), 0, 0, 0, 0,
           1));
}

// X軸中心の回転変換を表す行列を返す
Transform rotateX(const Real& theta) {
  const Real c = std::cos(theta);
  const Real s = std::sin(theta);
  Mat4 m(1, 0, 0, 0, 0, c, -s, 0, 0, s, c, 0, 0, 0, 0, 1);
  return Transform(m, transpose(m));
}

// Y軸中心の回転変換を表す行列を返す
Transform rotateY(const Real& theta) {
  const Real c = std::cos(theta);
  const Real s = std::sin(theta);
  Mat4 m(c, 0, s, 0, 0, 1, 0, 0, -s, 0, c, 0, 0, 0, 0, 1);
  return Transform(m, transpose(m));
}

// Z軸中心の回転変換を表す行列を返す
Transform rotateZ(const Real& theta) {
  const Real c = std::cos(theta);
  const Real s = std::sin(theta);
  Mat4 m(c, -s, 0, 0, s, c, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
  return Transform(m, transpose(m));
}

// それぞれの軸をr回転する変換を表す行列を返す
Transform rotate(const Vec3& r) {
  return rotateX(r.x()) * rotateY(r.y()) * rotateZ(r.z());
}

// 与えられたTransformの逆変換を表すTransformを返す
Transform inverse(const Transform& t) { return Transform(t.invmat, t.mat); }

}  // namespace Prl2
#endif