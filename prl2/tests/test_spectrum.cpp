#include <iostream>

#include "prl2.h"

using namespace Prl2;

int main() {
  SPD spd;
  spd.addPhi(542, 1);
  std::cout << spd << std::endl;
}