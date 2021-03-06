#include "shape/sphere.h"

#include <algorithm>

#include "core/constant.h"
#include "sampler/sampling.h"

namespace Prl2 {

//球のベクトル方程式にレイを代入
//二次方程式を解く
bool Sphere::intersect(const Ray& ray, IntersectInfo& info) const {
  const Real a = length2(ray.direction);
  const Real b = 2 * dot(ray.direction, ray.origin);
  const Real c = length2(ray.origin) - 1;

  const Real D = b * b - 4 * a * c;
  if (D < 0) return false;

  //桁落ちしない解の公式
  Real t0, t1;
  if (b > 0) {
    t0 = 0.5f * (-b - std::sqrt(D)) / a;
    t1 = c / (a * t0);
  } else {
    t1 = 0.5f * (-b + std::sqrt(D)) / a;
    t0 = c / (a * t1);
  }

  //近い方の解を計算
  Real t = t0;
  if (t < ray.tmin || t > ray.tmax) {
    t = t1;
    if (t < ray.tmin || t > ray.tmax) {
      return false;
    }
  }

  // ローカル座標系における衝突位置
  const Vec3 hitPos = ray(t);

  // 球面座標
  Real phi = std::atan2(hitPos.z(), hitPos.x());
  if (phi < 0) phi += PI_MUL_2;
  const Real theta = std::acos(std::clamp(hitPos.y(), -1.0f, 1.0f));

  //衝突情報を格納
  info.t = t;
  info.hitPos = hitPos;
  info.hitNormal = normalize(info.hitPos);
  info.uv = Vec2(phi * INV_PI_MUL_2, theta * INV_PI);
  return true;
}

bool Sphere::occluded(const Ray& ray) const {
  const Real a = length2(ray.direction);
  const Real b = 2 * dot(ray.direction, ray.origin);
  const Real c = length2(ray.origin) - 1;

  const Real D = b * b - 4 * a * c;
  if (D < 0) return false;

  //桁落ちしない解の公式
  Real t0, t1;
  if (b > 0) {
    t0 = 0.5f * (-b - std::sqrt(D)) / a;
    t1 = c / (a * t0);
  } else {
    t1 = 0.5f * (-b + std::sqrt(D)) / a;
    t0 = c / (a * t1);
  }

  //近い方の解を計算
  Real t = t0;
  if (t < ray.tmin || t > ray.tmax) {
    t = t1;
    if (t < ray.tmin || t > ray.tmax) {
      return false;
    }
  }

  return true;
}

void Sphere::samplePoint(Sampler& sampler, Vec3& p, Vec3& n,
                         Real& pdf_area) const {
  p = sampleSphere(sampler.getNext2D());
  n = normalize(p);
  pdf_area = INV_PI_MUL_4;
}

Bounds3 Sphere::getBounds() const { return Bounds3(Vec3(-1), Vec3(1)); }

}  // namespace Prl2