#include <iostream>

#include "parallel/parallel.h"

using namespace Prl2;

int main() {
  parallelFor1D([](int i) { std::cout << i << std::endl; }, 4, 16);

  return 0;
}