#ifndef PINHOLE_H
#define PINHOLE_H

#include "camera/camera.h"
#include "core/type.h"

namespace Prl2 {

class PinholeCamera : public Camera {
 public:
  PinholeCamera(const std::shared_ptr<Film>& _film,
                const std::shared_ptr<Transform>& _localToWorld,
                const Real& _fov)
      : Camera(_film, _localToWorld), fov(_fov), pinhole_distance(1){};

  bool generateRay(const Real& u, const Real& v, Ray& ray) const override;

 private:
  const Real fov;               //横視野角
  const Real pinhole_distance;  //ピンホールまでの距離
};

}  // namespace Prl2

#endif