#include "core/vec3.h"

using namespace Prl2;

int main() {
  Vec3 v(0, 1, 0);
  float theta, phi;
  cartesianToSpherical(v, theta, phi);
  std::cout << theta << ", " << phi << std::endl;
}
