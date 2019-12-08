#include "camera/thin-lens.h"
#include "core/util.h"
#include "sampler/sampling.h"

namespace Prl2 {

ThinLensCamera::ThinLensCamera(const std::shared_ptr<Film>& _film,
                               const std::shared_ptr<Transform>& _localToWorld,
                               const Real& _fov, const Real& _thin_lens_radius)
    : Camera(_film, _localToWorld),
      fov(degToRad(_fov)),
      thin_lens_radius(_thin_lens_radius) {
  thin_lens_pos = 0.5f * film->width_length / std::tan(0.5f * fov);
}

bool ThinLensCamera::generateRay(const Vec2& pFilm_2d, Sampler& sampler,
                                 Ray& ray, Real& pdf) const {
  const Vec3 pFilm(pFilm_2d.x(), pFilm_2d.y(), 0);

  const Vec2 pDisk = sampleDisk(sampler.getNext2D());
  const Vec3 pLens(thin_lens_radius * pDisk.x(), thin_lens_radius * pDisk.y(),
                   -thin_lens_pos);
  const Vec3 pLensCenter(0, 0, -thin_lens_pos);

  return false;
}

}  // namespace Prl2