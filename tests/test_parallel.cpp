#include <iostream>

#include "parallel/parallel.h"

using namespace Prl2;

int main() {
  // parallelFor1D([](int i) { std::cout << i << std::endl; }, 4, 16);
  parallelFor2D([](int x, int y) { std::cout << x << ", " << y << std::endl; },
                2, 2, 4, 4);

  return 0;
}