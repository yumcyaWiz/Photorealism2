#include "camera/pinhole.h"
#include "core/util.h"

namespace Prl2 {

PinholeCamera::PinholeCamera(const std::shared_ptr<Film>& _film,
                             const std::shared_ptr<Transform>& _localToWorld,
                             const Real& _fov)
    : Camera(_film, _localToWorld), fov(degToRad(_fov)) {
  pinhole_distance = 0.5f * film->width_length / std::tan(0.5f * fov);
}

bool PinholeCamera::generateRay(const Vec2& pFilm_2d, Sampler& sampler,
                                Ray& ray, Real& pdf) const {
  Vec3 pFilm(pFilm_2d.x(), pFilm_2d.y(), 0);
  Vec3 pinholePos = Vec3(0, 0, -pinhole_distance);

  //ローカル座標系のレイ
  ray.origin = pFilm;
  ray.direction = normalize(pinholePos - pFilm);
  //ワールド座標系に変換
  ray = localToWorld->apply(ray);

  pdf = 1;

  return true;
}

}  // namespace Prl2