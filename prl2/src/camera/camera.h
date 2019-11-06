#ifndef CAMERA_H
#define CAMERA_H

#include <memory>

#include "camera/film.h"
#include "core/transform.h"

namespace Prl2 {

class Camera {
 public:
  Camera(const std::shared_ptr<Film>& _film) : film(_film){};

 private:
  const std::shared_ptr<Transform>
      localToWorld;  //ローカル座標系からワールド座標系へのTransform
  const std::shared_ptr<Film> film;  //フィルム
};

}  // namespace Prl2
#endif