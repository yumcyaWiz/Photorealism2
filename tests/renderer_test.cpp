#include <memory>

#include "camera/pinhole.h"
#include "core/primitive.h"
#include "core/scene.h"
#include "core/spectrum.h"
#include "core/util.h"
#include "integrator/pt.h"
#include "intersector/linear.h"
#include "material/diffuse.h"
#include "renderer/renderer.h"
#include "sampler/random.h"
#include "shape/sphere.h"

using namespace Prl2;

int main() {
  RenderConfig config;
  RenderLayer layer(config);

  const auto film = std::make_shared<Film>(config.width, config.height);
  const auto camera_trans =
      std::make_shared<Transform>(translate(Vec3(0, 0, 0)));
  const auto camera =
      std::make_shared<PinholeCamera>(film, camera_trans, 90.0f);

  const auto shape = std::make_shared<Sphere>(1.0f);
  const auto mat = std::make_shared<Diffuse>(SPD(0.9));
  const auto prim_trans =
      std::make_shared<Transform>(translate(Vec3(0, 0, -3)));
  const auto prim = std::make_shared<Primitive>(shape, mat, prim_trans);

  const auto shape2 = std::make_shared<Sphere>(10000.0f);
  const auto prim2_trans =
      std::make_shared<Transform>(translate(Vec3(0, -10001, 0)));
  const auto prim2 = std::make_shared<Primitive>(shape2, mat, prim2_trans);

  std::shared_ptr<LinearIntersector> intersector =
      std::make_shared<LinearIntersector>();
  intersector->addPrimitive(prim);
  intersector->addPrimitive(prim2);

  Scene scene(camera, intersector);
  PT integrator;

  Renderer renderer;
  renderer.render(integrator, scene, config, layer);

  RGBArray2PPM(config.width, config.height, layer.render_sRGB, "output.ppm");

  return 0;
}