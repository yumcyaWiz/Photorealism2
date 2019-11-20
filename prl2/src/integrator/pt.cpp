#include "integrator/pt.h"

namespace Prl2 {

Real PT::integrate(const Ray& ray_in, const Scene& scene,
                   Sampler& sampler) const {
  Ray ray = ray_in;                // 更新していくレイ
  Real throughput = 1;             // Throughput
  Real russian_roulette_prob = 1;  // ロシアンルーレットの確率

  for (int depth = 0; depth < MAXDEPTH; ++depth) {
  }

  return 0;
}

}  // namespace Prl2