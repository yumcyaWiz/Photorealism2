#include "camera/thin-lens.h"
#include "core/util.h"

namespace Prl2 {

ThinLensCamera::ThinLensCamera(const std::shared_ptr<Film>& _film,
                               const std::shared_ptr<Transform>& _localToWorld)
    : Camera(_film, _localToWorld), fov(degToRad(90.0f)) {}

bool ThinLensCamera::generateRay(const Vec2& pFilm, Sampler& sampler, Ray& ray,
                                 Real& pdf) const {
  return false;
}

}  // namespace Prl2