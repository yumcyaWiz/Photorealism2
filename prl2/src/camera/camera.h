#ifndef CAMERA_H
#define CAMERA_H

#include <memory>

#include "core/transform.h"

namespace Prl2 {

class Camera {
 public:
  Camera(){};

 private:
  std::shared_ptr<Transform>
      localToWorld;  //ローカル座標系からワールド座標系へのTransform
};

}  // namespace Prl2
#endif