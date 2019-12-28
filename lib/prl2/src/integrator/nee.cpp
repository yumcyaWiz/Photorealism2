#include "integrator/nee.h"

namespace Prl2 {

bool NEE::integrate(int i, int j, const Scene& scene, Sampler& sampler,
                    IntegratorResult& result) const {
  // フィルム上の点のサンプリング
  Vec2 pFilm = scene.camera->sampleFilm(i, j, sampler);

  // Primary Rayを生成
  Ray ray;
  Real camera_cos, camera_pdf;
  if (!scene.camera->generateRay(pFilm, sampler, ray, camera_cos, camera_pdf)) {
    return false;
  }

  // 波長のサンプリング
  const Real lambda =
      SPD::LAMBDA_MIN + sampler.getNext() * (SPD::LAMBDA_MAX - SPD::LAMBDA_MIN);
  constexpr Real lambda_pdf = 1 / (SPD::LAMBDA_MAX - SPD::LAMBDA_MIN);
  ray.lambda = lambda;

  Real throughput = 1;                 // Throughput
  Real russian_roulette_prob = 0.99f;  // ロシアンルーレットの確率
  Real radiance = 0;                   // 分光放射輝度
  for (int depth = 0; depth < MAX_DEPTH; ++depth) {
    result.rays.push_back(ray);

    // ロシアンルーレット
    if (sampler.getNext() > russian_roulette_prob) {
      break;
    } else {
      throughput /= russian_roulette_prob;
    }

    // レイが物体に当たったら
    IntersectInfo info;
    if (scene.intersect(ray, info)) {
      // 光源に当たったら終了
      if (info.hitPrimitive->isLight()) {
        break;
      }

      const auto material = info.hitPrimitive->getMaterial();
      const Vec3 wo = -ray.direction;
      const Vec3 wo_local = worldToMaterial(wo, info);

      // Light Sampling
      const auto light = sampleLight(scene, sampler);
      Vec3 light_pos;
      Real light_pdf;
      light->samplePoint(info, sampler, light_pos, light_pdf);

      // Visibility Test
      Ray shadow_ray(info.hitPos, normalize(light_pos - info.hitPos),
                     ray.lambda);
      IntersectInfo shadow_info;
      if (scene.intersect(shadow_ray, shadow_info)) {
        if (shadow_info.hitPrimitive->getLight() == light) {
          MaterialArgs interaction;
          interaction.wo_local = wo_local;
          interaction.lambda = ray.lambda;

          const Real brdf = material->BRDF(interaction);
          const Real cos = std::abs(dot(shadow_ray.direction, info.hitNormal));
          radiance += throughput * brdf * cos *
                      light->Le(shadow_ray, shadow_info) / light_pdf;
        }
      }

      // BRDF Sampling
      MaterialArgs interaction;
      interaction.wo_local = wo_local;
      interaction.lambda = ray.lambda;
      Real pdf_w;
      const Real bsdf = material->sampleDirection(interaction, sampler, pdf_w);

      // Throughputを更新
      const Real cos = absCosTheta(interaction.wi_local);
      throughput *= bsdf * cos / pdf_w;

      // レイを更新
      const Vec3 wi_world = materialToWorld(interaction.wi_local, info);
      ray.origin = info.hitPos;
      ray.direction = wi_world;
    }
    // レイが空に飛んでいったら
    else {
      radiance += throughput * scene.sky->getRadiance(ray);
      break;
    }
  }

  result.lambda = lambda;
  result.phi = radiance * camera_cos / (lambda_pdf * camera_pdf);
  return true;
}

}  // namespace Prl2