#include "core/vec3.h"

using namespace Prl2;

int main() {
  Vec3 v(0, 1, 0);
  float theta, phi;
  cartesianToSpherical(v, theta, phi);
  std::cout << theta << ", " << phi << std::endl;

  Vec3 v1 = Vec3(0, 1, 0);
  Vec3 v2 = normalize(Vec3(1, 0, 0));
  std::cout << radianBetween(v1, v2) << std::endl;
}
