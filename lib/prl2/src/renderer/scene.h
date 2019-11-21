#ifndef SCENE_H
#define SCENE_H

#include <memory>

#include "camera/camera.h"
#include "core/isect.h"
#include "intersector/intersector.h"
#include "sky/sky.h"

namespace Prl2 {

//シーンを表現するクラス
// Camera, Intersectorを保持する
struct Scene {
 public:
  std::shared_ptr<Camera> camera;            // Camera
  std::shared_ptr<Intersector> intersector;  // Intersector
  std::shared_ptr<Sky> sky;                  // Sky

  Scene() : camera(nullptr), intersector(nullptr), sky(nullptr){};
  Scene(const std::shared_ptr<Camera> _camera,
        const std::shared_ptr<Intersector> _intersector,
        const std::shared_ptr<Sky> _sky)
      : camera(_camera), intersector(_intersector), sky(_sky){};

  bool intersect(const Ray& ray, IntersectInfo& info) const {
    return intersector->intersect(ray, info);
  };
};

}  // namespace Prl2
#endif