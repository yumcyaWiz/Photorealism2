#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "core/ray.h"
#include "core/spectrum.h"
#include "core/type.h"
#include "renderer/scene.h"
#include "sampler/sampler.h"

namespace Prl2 {

struct IntegratorResult {
  Real lambda;  // サンプリングされた波長
  Real phi;     // 分光放射輝度
};

//与えられたレイとシーンから分光放射輝度を計算するクラス
// Path Tracing, Path Tracing + MIS, Bidirectional Path Tracingなどを実装する
class Integrator {
 public:
  Integrator(){};

  // (i, j)の受け取る分光放射束を計算する
  virtual bool integrate(int i, int j, const Scene& scene, Sampler& sampler,
                         IntegratorResult& result) const = 0;
};

}  // namespace Prl2

#endif