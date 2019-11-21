#ifndef UTIL_H
#define UTIL_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "core/type.h"

namespace Prl2 {

void RGBArray2PPM(int width, int height, const std::vector<Real>& rgb,
                  const std::string& filename) {
  std::ofstream file(filename);
  file << "P3"
       << "\n"
       << width << " " << height << " "
       << "\n"
       << "255"
       << "\n";

  for (int j = 0; j < height; ++j) {
    for (int i = 0; i < width; ++i) {
      const int r = std::clamp(
          static_cast<int>(255 * rgb[0 + 3 * i + 3 * width * j]), 0, 255);
      const int g = std::clamp(
          static_cast<int>(255 * rgb[1 + 3 * i + 3 * width * j]), 0, 255);
      const int b = std::clamp(
          static_cast<int>(255 * rgb[2 + 3 * i + 3 * width * j]), 0, 255);
      file << r << " " << g << " " << b << "\n";
    }
  }

  file.close();
}

}  // namespace Prl2

#endif