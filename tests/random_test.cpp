#include <iostream>

#include "sampler/random.h"

using namespace Prl2;

int main() {
  Random random;

  for (int i = 0; i < 100; ++i) {
    std::cout << random.getNext() << std::endl;
  }

  return 0;
}