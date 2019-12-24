#include "integrator/integrator.h"

namespace Prl2 {

class NEE : public Integrator {
 public:
  NEE(){};

  bool integrate(int i, int j, const Scene& scene, Sampler& sampler,
                 IntegratorResult& result) const override;

 private:
  static constexpr int MAX_DEPTH = 100;
};

}  // namespace Prl2