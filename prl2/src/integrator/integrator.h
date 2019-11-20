#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "core/ray.h"
#include "core/scene.h"
#include "core/spectrum.h"
#include "core/type.h"
#include "sampler/sampler.h"

namespace Prl2 {

//与えられたレイとシーンから分光放射輝度を計算するクラス
// Path Tracing, Path Tracing + MIS, Bidirectional Path Tracingなどを実装する
class Integrator {
 public:
  Integrator(){};

  //与えられたレイとシーンから分光放射輝度を計算して返す
  virtual Real integrate(const Ray& ray, const Scene& scene,
                         Sampler& sampler) const = 0;
};

}  // namespace Prl2

#endif