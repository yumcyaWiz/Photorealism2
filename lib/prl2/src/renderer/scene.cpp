#include "renderer/scene.h"

namespace Prl2 {

Scene::Scene() : camera(nullptr), intersector(nullptr), sky(nullptr) {}

Scene::Scene(const std::shared_ptr<Camera> _camera,
             const std::shared_ptr<Intersector> _intersector,
             const std::shared_ptr<Sky> _sky)
    : camera(_camera), intersector(_intersector), sky(_sky) {}

void Scene::initScene() {
  // Initialize Intersector
  intersector->setPrimitives(primitives);
  intersector->initialize();

  // Initialize Light Array
  for (const auto& prim : primitives) {
    if (prim->isLight()) {
      lights.push_back(prim->getLight());
    }
  }
}

}  // namespace Prl2