#ifndef PT_H
#define PT_H

#include "integrator/integrator.h"

namespace Prl2 {

//純粋なPath Tracingを実装するクラス
class PT : public Integrator {
  PT(){};

  Real integrate(const Ray& ray, const Scene& scene) const override;
};

}  // namespace Prl2

#endif