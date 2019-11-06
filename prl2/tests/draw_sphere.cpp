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
  const auto prim_trans = std::make_shared<Transform>(translate(Vec3(0, 0, 3)));
  const auto prim = std::make_shared<Primitive>(shape, prim_trans);

  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < height; ++j) {
      const Real u = (2.0f * i - width) / width;
      const Real v = (2.0f * j - height) / height;
      Ray ray;
      if (camera->generateRay(i, j, ray)) {
        std::cout << ray << std::endl;
      }
    }
  }

  return 0;
}