#include "camera/environment.h"

namespace Prl2 {

EnvironmentCamera::EnvironmentCamera(
    const std::shared_ptr<Film>& _film,
    const std::shared_ptr<Transform>& _localToWorld)
    : Camera(_film, _localToWorld) {}

bool EnvironmentCamera::generateRay(const Vec2& pFilm, Sampler& sampler,
                                    Ray& ray, Real& pdf) const {
  // (u, v)を計算
  const Real u = pFilm.x() / (0.5f * film->width_length);
  const Real v = pFilm.y() / (0.5f * film->height_length);

  // 球面座標を計算
  const Real phi = 0.5f * (u + 1.0f) * PI_MUL_2;
  const Real theta = 0.5f * (v + 1.0f) * PI;

  // 球面座標からレイを生成
  ray.origin = Vec3(0);
  ray.direction = sphericalToCartesian(theta, phi);
  ray.direction[3] = -ray.direction[3];

  //ワールド座標系に変換
  ray = localToWorld->apply(ray);

  return true;
}

}  // namespace Prl2