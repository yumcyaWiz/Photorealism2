#include <iostream>
#include <memory>

#include "prl2.h"

using namespace Prl2;

int main() {
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