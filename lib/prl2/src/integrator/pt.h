#ifndef PT_H
#define PT_H

#include "integrator/integrator.h"

namespace Prl2 {

//純粋なPath Tracingを実装するクラス
class PT : public Integrator {
 public:
  PT(){};

  Real integrate(const Ray& ray, const Scene& scene,
                 Sampler& sampler) const override;

 private:
  static constexpr int MAXDEPTH = 100;  // 最大反射回数
};

}  // namespace Prl2

#endif