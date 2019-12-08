#ifndef _PRL2_ENVIRONMENT_H
#define _PRL2_ENVIRONMENT_H

#include "camera/camera.h"

namespace Prl2 {
class EnvironmentCamera : public Camera {
 public:
  EnvironmentCamera(const std::shared_ptr<Film>& _film,
                    const std::shared_ptr<Transform>& _localToWorld);

  bool generateRay(const Vec2& pFilm, Sampler& sampler, Ray& ray, Real& cos,
                   Real& pdf) const override;
};

}  // namespace Prl2

#endif