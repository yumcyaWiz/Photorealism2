#include "integrator/pt.h"

namespace Prl2 {

Real PT::integrate(const Ray& ray_in, const Scene& scene,
                   Sampler& sampler) const {
  Ray ray = ray_in;                    // 更新していくレイ
  Real throughput = 1;                 // Throughput
  Real russian_roulette_prob = 0.99f;  // ロシアンルーレットの確率

  for (int depth = 0; depth < MAXDEPTH; ++depth) {
    // ロシアンルーレット
    if (sampler.getNext() > russian_roulette_prob) {
      break;
    } else {
      throughput /= russian_roulette_prob;
    }

    // レイが物体に当たったら
    IntersectInfo info;
    if (scene.intersect(ray, info)) {
      // TODO: 光源だったら寄与を蓄積

      // BRDF Sampling
      const auto material = info.hitPrimitive->material;
      const Vec3 wo = -ray.direction;
      const Vec3 wo_local = worldToMaterial(wo, info);
      const SurfaceInteraction interaction(wo_local, ray.lambda, ray.ior);
      Vec3 wi_local;
      Real pdf_w;
      const Real bsdf =
          material->sampleDirection(interaction, sampler, wi_local, pdf_w);

      // Throughputを更新
      const Real cos = absCosTheta(wi_local);
      throughput *= bsdf * cos / pdf_w;

      // レイを更新
      const Vec3 wi_world = materialToWorld(wi_local, info);
      ray.origin = info.hitPos;
      ray.direction = wi_world;
    }
    // レイが空に飛んでいったら
    else {
      return throughput * scene.sky->getRadiance(ray);
    }
  }

  return 0;
}

}  // namespace Prl2