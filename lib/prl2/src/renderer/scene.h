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
  std::shared_ptr<Camera> camera;            // Camera
  std::shared_ptr<Intersector> intersector;  // Intersector

  Scene() : camera(nullptr), intersector(nullptr){};
  Scene(const std::shared_ptr<Camera> _camera, const std::shared_ptr<Intersector> _intersector) : camera(_camera), intersector(_intersector) {};

  void operator=(const Scene& scene) {
    camera = scene.camera;
    intersector = scene.intersector;
  };

  bool intersect(const Ray& ray, IntersectInfo& info) const {
    return intersector->intersect(ray, info);
  };
};

}  // namespace Prl2
#endif