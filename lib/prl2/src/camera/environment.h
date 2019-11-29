#ifndef _PRL2_ENVIRONMENT_H
#define _PRL2_ENVIRONMENT_H

#include "camera/camera.h"

namespace Prl2 {
class EnvironmentCamera : public Camera {
 public:
  EnvironmentCamera(const std::shared_ptr<Film>& _film,
                    const std::shared_ptr<Transform>& _localToWorld);

  bool generateRay(const Real& u, const Real& v, Ray& ray) const override;
};

}  // namespace Prl2

#endif