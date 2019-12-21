#ifndef _PRL2_AO_H
#define _PRL2_AO_H

#include "integrator/integrator.h"

namespace Prl2 {

class AO : public Integrator {
 public:
  AO(){};

  bool integrate(int i, int j, const Scene& scene, Sampler& sampler,
                 IntegratorResult& result) const override;
};

}  // namespace Prl2

#endif