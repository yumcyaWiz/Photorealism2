#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "core/ray.h"
#include "core/spectrum.h"
#include "core/type.h"
#include "renderer/scene.h"
#include "sampler/sampler.h"

namespace Prl2 {

struct IntegratorResult {
  Real lambda;            // サンプリングされた波長
  Real phi;               // 分光放射輝度
  std::vector<Ray> rays;  // Path

  IntegratorResult() : lambda(0), phi(0){};
};

//与えられたレイとシーンから分光放射輝度を計算するクラス
// Path Tracing, Path Tracing + MIS, Bidirectional Path Tracingなどを実装する
class Integrator {
 public:
  Integrator(){};

  // (i, j)の受け取る分光放射束を計算する
  virtual bool integrate(unsigned int i, unsigned int j, const Scene& scene,
                         Sampler& sampler, IntegratorResult& result) const = 0;

 protected:
  std::shared_ptr<Light> sampleLight(const Scene& scene,
                                     Sampler& sampler) const {
    unsigned int i = sampler.getNext() * scene.lights.size();
    if (i == scene.lights.size()) {
      i--;
    }
    return scene.lights[i];
  };
};

}  // namespace Prl2

#endif