#include "camera/thin-lens.h"
#include "core/util.h"
#include "sampler/sampling.h"

namespace Prl2 {

ThinLensCamera::ThinLensCamera(const std::shared_ptr<Film>& _film,
                               const std::shared_ptr<Transform>& _localToWorld,
                               const Real& _fov, const Real& _thin_lens_radius,
                               const Real& _focus_distance)
    : Camera(_film, _localToWorld),
      fov(degToRad(_fov)),
      thin_lens_radius(_thin_lens_radius),
      focus_distance(_focus_distance) {
  thin_lens_pos = 0.5f * film->width_length / std::tan(0.5f * fov);
}

bool ThinLensCamera::generateRay(const Vec2& pFilm_2d, Sampler& sampler,
                                 Ray& ray, Real& pdf) const {
  // フィルム上の点
  const Vec3 pFilm(pFilm_2d.x(), pFilm_2d.y(), 0);

  // レンズ上の点をサンプリング
  const Vec2 pDisk = sampleDisk(sampler.getNext2D());
  const Vec3 pLens(thin_lens_radius * pDisk.x(), thin_lens_radius * pDisk.y(),
                   -thin_lens_pos);

  // ピントの合う位置を計算
  const Vec3 pLensCenter(0, 0, -thin_lens_pos);
  const Vec3 filmToLensCenter = normalize(pLensCenter - pFilm);
  const Real cos = std::abs(filmToLensCenter.z());
  const Real r = focus_distance / cos;
  const Vec3 objectPos = pFilm + r * filmToLensCenter;

  // レイの生成
  ray.origin = pLens;
  ray.direction = normalize(objectPos - pLens);
  pdf = (thin_lens_pos * thin_lens_pos) / (cos * cos * cos) * 1 /
        (thin_lens_radius * thin_lens_radius);

  return false;
}

}  // namespace Prl2