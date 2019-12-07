#include "camera/thin-lens.h"
#include "core/util.h"

namespace Prl2 {

ThinLensCamera::ThinLensCamera(const std::shared_ptr<Film>& _film,
                               const std::shared_ptr<Transform>& _localToWorld)
    : Camera(_film, _localToWorld), fov(degToRad(90.0f)) {}

bool ThinLensCamera::generateRay(const Real& u, const Real& v, Ray& ray) const {
  return false;
}

}  // namespace Prl2