#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "core/ray.h"
#include "core/scene.h"
#include "core/spectrum.h"

namespace Prl2 {

//与えられたレイとシーンから分光放射束を計算して返すクラス
// Path Tracing, Path Tracing + MIS, Bidirectional Path Tracingなどを実装する
class Integrator {
 public:
  Integrator(){};

  //与えられたレイとシーンから分光放射束を計算して返す
  virtual Real integrate(const Ray& ray, const Scene& scene,
                         SPD& spd) const = 0;
};

};  // namespace Prl2

#endif