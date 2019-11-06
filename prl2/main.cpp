#include <iostream>
#include <memory>

#include "prl2.h"

using namespace Prl2;

int main() {
  /*
  Prl2::Film film(512, 512);
  for (std::size_t i = 0; i < film.width; ++i) {
    for (std::size_t j = 0; j < film.height; ++j) {
      const Prl2::Real l = float(i) / film.width * (780 - 380) + 380;
      std::vector<Prl2::Real> lambda{l - 5, l, l + 5};
      std::vector<Prl2::Real> phi{0, 1, 0};
      Prl2::SPD spd(lambda, phi);
      film.addPixel(i, j, spd);
    }
  }
  film.writePPM("output.ppm");
  */
  auto shape = std::make_shared<Sphere>(1.0f);
  auto transform = std::make_shared<Transform>(translate(Vec3(0, 0, 3)));
  auto prim = std::make_shared<Primitive>(shape, transform);

  const Ray ray(Vec3(0, 0, 0), Vec3(0, 0, 1), 0);
  IntersectInfo info;

  if (prim->intersect(ray, info)) {
    std::cout << info << std::endl;
  }

  return 0;
}