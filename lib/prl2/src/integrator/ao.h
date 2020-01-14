#ifndef _PRL2_AO_H
#define _PRL2_AO_H

#include "integrator/integrator.h"

namespace Prl2 {

class AO : public Integrator {
 public:
  AO() noexcept : white(10 * D65Light()){};

  bool integrate(unsigned int i, unsigned int j, const Scene& scene,
                 Sampler& sampler, IntegratorResult& result) const override;

 private:
  const SPD white;
};

}  // namespace Prl2

#endif