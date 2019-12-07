#ifndef _PRL2_EXAMPLE_H
#define _PRL2_EXAMPLE_H

#include "camera/camera.h"

namespace Prl2 {

class ThinLensCamera : public Camera {
 public:
  ThinLensCamera(const std::shared_ptr<Film>& _film,
                 const std::shared_ptr<Transform>& _localToWorld);

  bool generateRay(const Real& u, const Real& v, Ray& ray) const override;

 private:
  const Real fov;  // 横視野角
};

}  // namespace Prl2

#endif