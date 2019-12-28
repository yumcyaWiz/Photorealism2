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
  result.lambda = lambda;

  const Real white_phi = white.sample(lambda);

  IntersectInfo info;
  if (scene.intersect(ray, info)) {
    // Sample Ray Direction
    const Vec3 wi_local = sampleHemisphere(sampler.getNext2D());
    Vec3 s, t;
    orthonormalBasis(info.hitNormal, s, t);
    const Vec3 wi = materialToWorld(wi_local, s, info.hitNormal, t);
    Ray shadow_ray(info.hitPos, wi);

    // Compute Hit Distance
    IntersectInfo shadow_info;
    Real hitDistance = ray.tmax;
    if (scene.intersect(shadow_ray, shadow_info)) {
      hitDistance = shadow_info.t;
    }

    result.phi = hitDistance > 1 ? white_phi : 0;
  } else {
    result.phi = 0;
  }

  return true;
}

}  // namespace Prl2