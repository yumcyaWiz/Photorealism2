#include "camera/film.h"
#include "camera/pinhole.h"
#include "core/primitive.h"
#include "core/scene.h"
#include "renderer/renderer.h"
#include "shape/sphere.h"

using namespace Prl2;

int main() {
  RenderConfig config;

  RenderLayer layer(config);

  Renderer renderer;

  const auto film = std::make_shared<Film>(config.width, config.height);
  const auto camera_trans =
      std::make_shared<Transform>(translate(Vec3(0, 0, 0)));
  const auto camera =
      std::make_shared<PinholeCamera>(film, camera_trans, 90.0f);

  const auto shape = std::make_shared<Sphere>(1.0f);
  const auto prim_trans =
      std::make_shared<Transform>(translate(Vec3(0, 0, -3)));
  const auto prim = std::make_shared<Primitive>(shape, prim_trans);
}