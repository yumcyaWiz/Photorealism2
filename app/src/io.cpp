#include "io.h"

#include <algorithm>
#include <fstream>
#include <iostream>

void savePPM(const std::string& filename, int width, int height,
             const std::vector<float>& rgb) {
  std::ofstream file(filename);

  file << "P3" << std::endl;
  file << width << " " << height << std::endl;
  file << "255" << std::endl;

  for (int j = 0; j < height; ++j) {
    for (int i = 0; i < width; ++i) {
      const int r = std::clamp(
          static_cast<int>(255 * rgb[3 * i + 3 * width * j + 0]), 0, 255);
      const int g = std::clamp(
          static_cast<int>(255 * rgb[3 * i + 3 * width * j + 1]), 0, 255);
      const int b = std::clamp(
          static_cast<int>(255 * rgb[3 * i + 3 * width * j + 2]), 0, 255);
      file << r << " " << g << " " << b << std::endl;
    }
  }

  file.close();
}