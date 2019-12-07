#ifndef _PRL2_EXAMPLE_H
#define _PRL2_EXAMPLE_H

#include "camera/camera.h"

namespace Prl2 {

class ThinLensCamera : public Camera {
 public:
  ThinLensCamera(const std::shared_ptr<Film>& _film,
                 const std::shared_ptr<Transform>& _localToWorld,
                 const Real& _fov, const Real& _thin_lens_radius);

  bool generateRay(const Vec2& pFilm, Sampler& sampler, Ray& ray,
                   Real& pdf) const override;

 private:
  const Real fov;               // 横視野角
  const Real thin_lens_radius;  // Thin-Lensの半径
  Real thin_lens_pos;           // Thin-Lensの位置
};

}  // namespace Prl2

#endif