#include "camera/pinhole.h"
#include "core/util.h"

namespace Prl2 {

PinholeCamera::PinholeCamera(const std::shared_ptr<Film>& _film,
                             const std::shared_ptr<Transform>& _localToWorld,
                             const Real& _fov)
    : Camera(_film, _localToWorld), fov(degToRad(_fov)) {
  pinhole_distance = 0.5f * film->width_length / std::tan(0.5f * fov);
}

bool PinholeCamera::generateRay(const Real& u, const Real& v, Ray& ray) const {
  assert(u >= -1 && u <= 1);
  assert(v >= -1 && v <= 1);

  Vec2 pFilm_2d = film->computePosition(u, v);
  Vec3 pFilm(pFilm_2d.x(), pFilm_2d.y(), 0);
  Vec3 pinholePos = Vec3(0, 0, -pinhole_distance);

  //ローカル座標系のレイ
  ray.origin = pFilm;
  ray.direction = normalize(pinholePos - pFilm);
  //ワールド座標系に変換
  ray = localToWorld->apply(ray);

  return true;
}

}  // namespace Prl2