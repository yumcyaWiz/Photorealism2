#include "integrator/ao.h"

#include "sampler/sampling.h"

namespace Prl2 {

bool AO::integrate(int i, int j, const Scene& scene, Sampler& sampler,
                   IntegratorResult& result) const {
  // フィルム上の点のサンプリング
  Vec2 pFilm = scene.camera->sampleFilm(i, j, sampler);

  // Primary Rayを生成
  Ray ray;
  Real camera_cos, camera_pdf;
  if (!scene.camera->generateRay(pFilm, sampler, ray, camera_cos, camera_pdf)) {
    return false;
  }

  // 波長は550nmに固定
  // 波長のサンプリング
  const Real lambda =
      SPD::LAMBDA_MIN + sampler.getNext() * (SPD::LAMBDA_MAX - SPD::LAMBDA_MIN);
  constexpr Real lambda_pdf = 1 / (SPD::LAMBDA_MAX - SPD::LAMBDA_MIN);
  ray.lambda = lambda;
  result.lambda = lambda;

  const Real white_phi = white.sample(ray.lambda);

  IntersectInfo info;
  if (scene.intersect(ray, info)) {
    // Sample Ray Direction
    const Vec3 wi_local = sampleHemisphere(sampler.getNext2D());
    const Vec3 wi = materialToWorld(wi_local, info);
    Ray shadow_ray(info.hitPos, wi);

    // Compute Hit Distance
    IntersectInfo shadow_info;
    Real hitDistance = 0;
    if (scene.intersect(shadow_ray, shadow_info)) {
      hitDistance = shadow_info.t;
    } else {
      hitDistance = ray.tmax;
    }

    if (hitDistance > 1) {
      result.phi = white_phi;
    }
  } else {
    result.phi = white_phi;
  }

  return true;
}

}  // namespace Prl2