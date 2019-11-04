#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "mat4.h"
#include "vec3.h"

class Transform {
 public:
  Mat4 mat;
  Mat4 invmat;

  Transform() : mat(identity()), invmat(identity()){};
  Transform(const Mat4& _mat, const Mat4& _invmat)
      : mat(_mat), invmat(_invmat){};

  Transform operator*(const Transform& t) const {
    return Transform(mat * t.mat, invmat * t.invmat);
  };

  Vec3 applyDirection(const Vec3& v) const {
    return Vec3(
        mat.m[0][0] + v.x() + mat.m[0][1] * v.y() + mat.m[0][2] * v.z(),
        mat.m[1][0] * v.x() + mat.m[1][1] * v.y() + mat.m[1][2] * v.z(),
        mat.m[2][0] * v.x() + mat.m[2][1] * v.y() + mat.m[2][2] * v.z());
  };

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

  Vec3 applyNormal(const Vec3& n) const {
    return Vec3(invmat.m[0][0] * n.x() + invmat.m[1][0] * n.y() +
                    invmat.m[2][0] * n.z(),
                invmat.m[0][1] * n.x() + invmat.m[1][1] * n.y() +
                    invmat.m[2][1] * n.z(),
                invmat.m[0][2] * n.x() + invmat.m[1][1] * n.y() +
                    invmat.m[2][2] * n.z());
  };
};

Transform translate(const Vec3& v) {
  return Transform(
      Mat4(1, 0, 0, v.x(), 0, 1, 0, v.y(), 0, 0, 1, v.z(), 0, 0, 0, 1),
      Mat4(1, 0, 0, -v.x(), 0, 1, 0, -v.y(), 0, 0, 1, -v.z(), 0, 0, 0, 1));
}

Transform scale(const Vec3& s) {
  return Transform(
      Mat4(s.x(), 0, 0, 0, 0, s.y(), 0, 0, 0, 0, s.z(), 0, 0, 0, 0, 1),
      Mat4(1 / s.x(), 0, 0, 0, 0, 1 / s.y(), 0, 0, 0, 0, 1 / s.z(), 0, 0, 0, 0,
           1));
}

Transform rotateX(const Real& theta) {
  const Real c = std::cos(theta);
  const Real s = std::sin(theta);
  Mat4 m(1, 0, 0, 0, 0, c, -s, 0, 0, s, c, 0, 0, 0, 0, 1);
  return Transform(m, transpose(m));
}

Transform rotateY(const Real& theta) {
  const Real c = std::cos(theta);
  const Real s = std::sin(theta);
  Mat4 m(c, 0, s, 0, 0, 1, 0, 0, -s, 0, c, 0, 0, 0, 0, 1);
  return Transform(m, transpose(m));
}

Transform rotateZ(const Real& theta) {
  const Real c = std::cos(theta);
  const Real s = std::sin(theta);
  Mat4 m(c, -s, 0, 0, s, c, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
  return Transform(m, transpose(m));
}

Transform rotate(const Vec3& r) {
  return rotateX(r.x()) * rotateY(r.y()) * rotateZ(r.z());
}

Transform inverse(const Transform& t) { return Transform(t.invmat, t.mat); }
#endif