#ifndef PRL2_TRANSFORM_H
#define PRL2_TRANSFORM_H

#include "core/bounds3.h"
#include "core/isect.h"
#include "core/mat4.h"
#include "core/ray.h"
#include "core/util.h"
#include "core/vec3.h"

namespace Prl2 {

//アフィン変換を行うクラス
class Transform {
 public:
  Mat4 mat;     //変換行列
  Mat4 invmat;  //逆変換行列

  Transform() : mat(identity()), invmat(identity()) {}
  Transform(const Mat4& _mat, const Mat4& _invmat)
      : mat(_mat), invmat(_invmat) {}
  Transform(const Transform& t) noexcept : mat(t.mat), invmat(t.invmat) {}

  Transform operator*(const Transform& t) const {
    const auto t2 = Transform(mat * t.mat, t.invmat * invmat);
    return t2;
  }

  Transform& operator=(const Transform& t) {
    mat = t.mat;
    invmat = t.invmat;
    return *this;
  }

  // 方向ベクトルに対して変換を施す
  Vec3 applyDirection(const Vec3& v) const {
    Vec3 ret;
    ret[0] = mat.m[0][0] * v.x() + mat.m[0][1] * v.y() + mat.m[0][2] * v.z();
    ret[1] = mat.m[1][0] * v.x() + mat.m[1][1] * v.y() + mat.m[1][2] * v.z();
    ret[2] = mat.m[2][0] * v.x() + mat.m[2][1] * v.y() + mat.m[2][2] * v.z();
    return ret;
  }

  // 方向ベクトルに対して逆変換を施す
  Vec3 applyDirectionInverse(const Vec3& v) const {
    Vec3 ret;
    ret[0] = invmat.m[0][0] * v.x() + invmat.m[0][1] * v.y() +
             invmat.m[0][2] * v.z();
    ret[1] = invmat.m[1][0] * v.x() + invmat.m[1][1] * v.y() +
             invmat.m[1][2] * v.z();
    ret[2] = invmat.m[2][0] * v.x() + invmat.m[2][1] * v.y() +
             invmat.m[2][2] * v.z();
    return ret;
  }

  // 点ベクトルに対して変換を施す
  Vec3 applyPoint(const Vec3& v) const {
    Vec3 ret;
    const Real x = mat.m[0][0] * v.x() + mat.m[0][1] * v.y() +
                   mat.m[0][2] * v.z() + mat.m[0][3];
    const Real y = mat.m[1][0] * v.x() + mat.m[1][1] * v.y() +
                   mat.m[1][2] * v.z() + mat.m[1][3];
    const Real z = mat.m[2][0] * v.x() + mat.m[2][1] * v.y() +
                   mat.m[2][2] * v.z() + mat.m[2][3];
    const Real w = mat.m[3][0] * v.x() + mat.m[3][1] * v.y() +
                   mat.m[3][2] * v.z() + mat.m[3][3];

    if (equalf(w, 1)) {
      ret[0] = x;
      ret[1] = y;
      ret[2] = z;
    } else {
      ret[0] = x / w;
      ret[1] = y / w;
      ret[2] = z / w;
    }

    return ret;
  }
  // 点ベクトルに対して逆変換を施す
  Vec3 applyPointInverse(const Vec3& v) const {
    Vec3 ret;
    const Real x = invmat.m[0][0] * v.x() + invmat.m[0][1] * v.y() +
                   invmat.m[0][2] * v.z() + invmat.m[0][3];
    const Real y = invmat.m[1][0] * v.x() + invmat.m[1][1] * v.y() +
                   invmat.m[1][2] * v.z() + invmat.m[1][3];
    const Real z = invmat.m[2][0] * v.x() + invmat.m[2][1] * v.y() +
                   invmat.m[2][2] * v.z() + invmat.m[2][3];
    const Real w = invmat.m[3][0] * v.x() + invmat.m[3][1] * v.y() +
                   invmat.m[3][2] * v.z() + invmat.m[3][3];

    if (equalf(w, 1)) {
      ret[0] = x;
      ret[1] = y;
      ret[2] = z;
    } else {
      ret[0] = x / w;
      ret[1] = y / w;
      ret[2] = z / w;
    }

    return ret;
  }

  // 法線ベクトルに対して変換を施す
  Vec3 applyNormal(const Vec3& n) const {
    Vec3 ret;
    ret[0] = invmat.m[0][0] * n.x() + invmat.m[1][0] * n.y() +
             invmat.m[2][0] * n.z();
    ret[1] = invmat.m[0][1] * n.x() + invmat.m[1][1] * n.y() +
             invmat.m[2][1] * n.z();
    ret[2] = invmat.m[0][2] * n.x() + invmat.m[1][2] * n.y() +
             invmat.m[2][2] * n.z();

    return ret;
  }
  // 法線ベクトルに対して逆変換を施す
  Vec3 applyNormalInverse(const Vec3& n) const {
    Vec3 ret;
    ret[0] = mat.m[0][0] * n.x() + mat.m[1][0] * n.y() + mat.m[2][0] * n.z();
    ret[1] = mat.m[0][1] * n.x() + mat.m[1][1] * n.y() + mat.m[2][1] * n.z();
    ret[2] = mat.m[0][2] * n.x() + mat.m[1][2] * n.y() + mat.m[2][2] * n.z();
    return ret;
  }

  //レイに対して変換を施す
  Ray apply(const Ray& ray) const {
    Ray ret = ray;
    ret.origin = applyPoint(ray.origin);
    ret.direction = applyDirection(ray.direction);
    return ret;
  }
  //レイに対して逆変換を施す
  Ray applyInverse(const Ray& ray) const {
    Ray ret = ray;
    ret.origin = applyPointInverse(ray.origin);
    ret.direction = applyDirectionInverse(ray.direction);
    return ret;
  }

  // IntersectInfoに対して変換を施す
  IntersectInfo apply(const IntersectInfo& isect) const {
    IntersectInfo ret = isect;
    ret.hitPos = applyPoint(isect.hitPos);
    ret.hitNormal = normalize(applyNormal(isect.hitNormal));
    return ret;
  }

  // Bounds3に対して変換を施す
  Bounds3 apply(const Bounds3& bounds) const {
    return Bounds3(applyPoint(bounds.p0), applyPoint(bounds.p1));
  }
};

//移動変換を表す行列を返す
inline Transform translate(const Vec3& v) {
  return Transform(
      Mat4(1, 0, 0, v.x(), 0, 1, 0, v.y(), 0, 0, 1, v.z(), 0, 0, 0, 1),
      Mat4(1, 0, 0, -v.x(), 0, 1, 0, -v.y(), 0, 0, 1, -v.z(), 0, 0, 0, 1));
}

//スケール変換を表す行列を返す
inline Transform scale(const Vec3& s) {
  return Transform(
      Mat4(s.x(), 0, 0, 0, 0, s.y(), 0, 0, 0, 0, s.z(), 0, 0, 0, 0, 1),
      Mat4(1 / s.x(), 0, 0, 0, 0, 1 / s.y(), 0, 0, 0, 0, 1 / s.z(), 0, 0, 0, 0,
           1));
}

// X軸中心の回転変換を表す行列を返す
inline Transform rotateX(const Real& theta) {
  const Real c = std::cos(theta);
  const Real s = std::sin(theta);
  Mat4 m(1, 0, 0, 0, 0, c, -s, 0, 0, s, c, 0, 0, 0, 0, 1);
  return Transform(m, transpose(m));
}

// Y軸中心の回転変換を表す行列を返す
inline Transform rotateY(const Real& theta) {
  const Real c = std::cos(theta);
  const Real s = std::sin(theta);
  Mat4 m(c, 0, s, 0, 0, 1, 0, 0, -s, 0, c, 0, 0, 0, 0, 1);
  return Transform(m, transpose(m));
}

// Z軸中心の回転変換を表す行列を返す
inline Transform rotateZ(const Real& theta) {
  const Real c = std::cos(theta);
  const Real s = std::sin(theta);
  Mat4 m(c, -s, 0, 0, s, c, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
  return Transform(m, transpose(m));
}

// それぞれの軸をr回転する変換を表す行列を返す
inline Transform rotate(const Vec3& r) {
  return rotateX(r.x()) * rotateY(r.y()) * rotateZ(r.z());
}

// 任意の軸中心に回転する変換を表す行列を返す
inline Transform rotate(const Real& theta, const Vec3& axis) {
  const Vec3 a = normalize(axis);
  const Real s = std::sin(theta);
  const Real c = std::cos(theta);

  Mat4 m;
  m.m[0][0] = a.x() * a.x() + (1 - a.x() * a.x()) * c;
  m.m[0][1] = a.x() * a.y() * (1 - c) - a.z() * s;
  m.m[0][2] = a.x() * a.z() * (1 - c) + a.y() * s;
  m.m[0][3] = 0;

  m.m[1][0] = a.x() * a.y() * (1 - c) + a.z() * s;
  m.m[1][1] = a.y() * a.y() + (1 - a.y() * a.y()) * c;
  m.m[1][2] = a.y() * a.z() * (1 - c) - a.x() * s;
  m.m[1][3] = 0;

  m.m[2][0] = a.x() * a.z() * (1 - c) - a.y() * s;
  m.m[2][1] = a.y() * a.z() * (1 - c) + a.x() * s;
  m.m[2][2] = a.z() * a.z() + (1 - a.z() * a.z()) * c;
  m.m[2][3] = 0;

  m.m[3][0] = 0;
  m.m[3][1] = 0;
  m.m[3][2] = 0;
  m.m[3][3] = 1;

  return Transform(m, transpose(m));
}

// LookAt Transformを返す
inline Transform lookAt(const Vec3& pos, const Vec3& lookat,
                        const Vec3& up_base = Vec3(0, 1, 0)) {
  const Vec3 forward = -normalize(lookat - pos);
  const Vec3 right = normalize(cross(forward, normalize(up_base)));
  const Vec3 up = normalize(cross(right, forward));

  Mat4 m;
  m.m[0][0] = right.x();
  m.m[0][1] = up.x();
  m.m[0][2] = forward.x();
  m.m[0][3] = pos.x();
  m.m[1][0] = right.y();
  m.m[1][1] = up.y();
  m.m[1][2] = forward.y();
  m.m[1][3] = pos.y();
  m.m[2][0] = right.z();
  m.m[2][1] = up.z();
  m.m[2][2] = forward.z();
  m.m[2][3] = pos.z();
  m.m[3][0] = 0;
  m.m[3][1] = 0;
  m.m[3][2] = 0;
  m.m[3][3] = 1;

  return Transform(m, identity());
}

// 与えられたTransformの逆変換を表すTransformを返す
inline Transform inverse(const Transform& t) {
  return Transform(t.invmat, t.mat);
}

inline std::ostream& operator<<(std::ostream& stream, const Transform& t) {
  stream << "Forward: " << t.mat << std::endl;
  stream << "Inverse: " << t.invmat << std::endl;
  return stream;
}

}  // namespace Prl2
#endif