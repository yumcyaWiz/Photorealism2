#include <memory>

#include "prl2.h"

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
  const auto prim_trans =
      std::make_shared<Transform>(translate(Vec3(0, 0, -3)));
  const auto prim = std::make_shared<Primitive>(shape, prim_trans);

  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < height; ++j) {
      const Real u = (2.0f * i - width) / width;
      const Real v = (2.0f * j - height) / height;
      Ray ray;
      if (camera->generateRay(u, v, ray)) {
        IntersectInfo info;
        if (prim->intersect(ray, info)) {
          const std::vector<Real> lambda = {450, 550, 610};
          SPD spd;
          spd.addPhi(450, 0.25f * (info.hitNormal.z() + 1.0f));
          spd.addPhi(550, 0.25f * (info.hitNormal.y() + 1.0f));
          spd.addPhi(610, 0.25f * (info.hitNormal.x() + 1.0f));
          film->addPixel(i, j, spd);
        }
      }
    }
  }

  film->writePPM("output.ppm");

  return 0;
}