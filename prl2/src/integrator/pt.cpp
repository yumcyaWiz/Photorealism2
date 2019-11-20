#include "integrator/pt.h"

namespace Prl2 {

Real PT::integrate(const Ray& ray_in, const Scene& scene,
                   Sampler& sampler) const {
  Ray ray = ray_in;                // 更新していくレイ
  Real throughput = 1;             // Throughput
  Real russian_roulette_prob = 1;  // ロシアンルーレットの確率

  for (int depth = 0; depth < MAXDEPTH; ++depth) {
    // TODO: ロシアンルーレット

    // レイが物体に当たったら
    IntersectInfo info;
    if (scene.intersect(ray, info)) {
      // TODO: 光源だったら寄与を蓄積

      // BRDF Sampling
      SurfaceInteraction interaction(ray, info);
      auto material = info.hitPrimitive->material;
      Real pdf_w;
      const Real bsdf = material->sampleDirection(interaction, sampler, pdf_w);

      // Throughputを更新
      const float cos = absCosTheta(interaction.wi);
      throughput *= bsdf * cos / pdf_w;

      // レイを更新
      const Vec3 wi_world = materialToWorld(interaction.wi, info);
      ray.origin = info.hitPos;
      ray.direction = wi_world;
    }
    // レイが空に飛んでいったら
    else {
      return throughput * 0.01;
    }
  }

  return 0;
}

}  // namespace Prl2