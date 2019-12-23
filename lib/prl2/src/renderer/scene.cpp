#include "renderer/scene.h"

namespace Prl2 {

Scene::Scene() : camera(nullptr), intersector(nullptr), sky(nullptr) {}

Scene::Scene(const std::shared_ptr<Camera> _camera,
             const std::shared_ptr<Intersector> _intersector,
             const std::shared_ptr<Sky> _sky)
    : camera(_camera), intersector(_intersector), sky(_sky) {}

}  // namespace Prl2