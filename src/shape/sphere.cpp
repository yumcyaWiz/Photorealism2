#include "shape/sphere.h"

//球のベクトル方程式にレイを代入
//二次方程式を解く
bool Sphere::intersect(const Ray& ray, IntersectInfo& info) const {
  const Real a = length(ray.direction);
  const Real b = 2 * dot(ray.direction, ray.origin);
  const Real c = length2(ray.origin) - radius * radius;

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
    if (t < ray.tmin || ray.tmax) {
      return false;
    }
  }

  info.t = t;
  info.hitPos = ray(t);
  info.hitNormal = normalize(info.hitPos);
}