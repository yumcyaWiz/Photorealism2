#ifndef PINHOLE_H
#define PINHOLE_H

#include "camera/camera.h"
#include "core/type.h"

namespace Prl2 {

class PinholeCamera : public Camera {
 public:
  PinholeCamera(const std::shared_ptr<Film>& _film,
                const std::shared_ptr<Transform>& _localToWorld,
                const Real& _fov);

  bool generateRay(const Vec2& pFilm, Sampler& sampler, Ray& ray, Real& cos,
                   Real& pdf) const override;

 private:
  const Real fov;         //横視野角
  Real pinhole_distance;  //ピンホールまでの距離
};

}  // namespace Prl2

#endif