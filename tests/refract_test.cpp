#include "material/material.h"

using namespace Prl2;

int main() {
  Vec3 wo = normalize(Vec3(0.2, -1, 0));
  Vec3 wt;
  refract(wo, wt, Vec3(0, -1, 0), 1.5, 1);
  std::cout << wt << std::endl;

  return 0;
}