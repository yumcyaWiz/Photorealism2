#include "integrator/pt.h"

namespace Prl2 {

Real PT::integrate(const Ray& ray_in, const Scene& scene,
                   Sampler& sampler) const {
  Ray ray = ray_in;                    // 更新していくレイ
  Real throughput = 1;                 // Throughput
  Real russian_roulette_prob = 0.99f;  // ロシアンルーレットの確率
  Real radiance = 0;                   // 分光放射輝度

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
      // 光源に当たったら寄与を追加
      if (info.hitPrimitive->light != nullptr) {
        radiance += throughput * info.hitPrimitive->light->Le(ray, info);
      }

      // BRDF Sampling
      const auto material = info.hitPrimitive->material;
      const Vec3 wo = -ray.direction;
      const Vec3 wo_local = worldToMaterial(wo, info);
      const SurfaceInteraction interaction(wo_local, ray.lambda);
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
      radiance += throughput * scene.sky->getRadiance(ray);
      break;
    }
  }

  return radiance;
}

}  // namespace Prl2