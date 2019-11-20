#include <iostream>

#include "sampler/random.h"
#include "sampler/sampling.h"

using namespace Prl2;

int main() {
  Random random;

  for (int i = 0; i < 100; ++i) {
    Vec2 u = random.getNext2D();
    std::cout << u << std::endl;
  }

  return 0;
}