#ifndef SCENE_H
#define SCENE_H

#include <memory>

#include "camera/camera.h"
#include "core/isect.h"
#include "intersector/intersector.h"

namespace Prl2 {

//シーンを表現するクラス
// Camera, Intersectorを保持する
struct Scene {
 public:
  const std::shared_ptr<Camera> camera;            // Camera
  const std::shared_ptr<Intersector> intersector;  // Intersector

  Scene(const std::shared_ptr<Camera>& _camera,
        const std::shared_ptr<Intersector>& _intersector)
      : camera(_camera), intersector(_intersector){};
};

};  // namespace Prl2
#endif