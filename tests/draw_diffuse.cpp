#include <memory>

#include "camera/pinhole.h"
#include "core/primitive.h"
#include "core/scene.h"
#include "core/spectrum.h"
#include "integrator/pt.h"
#include "intersector/linear.h"
#include "material/diffuse.h"
#include "sampler/random.h"
#include "shape/sphere.h"

using namespace Prl2;

int main() {
  const int width = 512;
  const int height = 512;

  const auto film = std::make_shared<Film>(width, height);
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
  RandomSampler sampler;

  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < height; ++j) {
      const Real u = (2.0f * i - width) / width;
      const Real v = (2.0f * j - height) / height;

      for (int k = 0; k < 100; ++k) {
        // 波長のサンプリング
        Real lambda = SPD::LAMBDA_MIN + 0.99 * sampler.getNext() *
                                            (SPD::LAMBDA_MAX - SPD::LAMBDA_MIN);
        Ray ray;
        ray.lambda = lambda;
        if (camera->generateRay(u, v, ray)) {
          Real phi = integrator.integrate(ray, scene, sampler);
          film->addPixel(i, j, ray.lambda, phi);
        }
      }

      film->divide(i, j, 100);
    }
  }

  film->writePPM("output.ppm");

  return 0;
}