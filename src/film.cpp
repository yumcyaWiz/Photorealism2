#include "film.h"

#include <cmath>

void Film::writePPM(const std::string& filename) const {
  std::ofstream file(filename);

  file << "P3" << std::endl;
  file << width << " " << height << std::endl;
  file << "255" << std::endl;

  for (std::size_t j = 0; j < height; ++j) {
    for (std::size_t i = 0; i < width; ++i) {
      const SPD spd = getPixel(i, j);

      // RGBを計算
      const RGB rgb = spd.toRGB();
      const unsigned char r = std::clamp(
          static_cast<unsigned char>(255 * rgb.x),
          static_cast<unsigned char>(0), static_cast<unsigned char>(255));
      const unsigned char g = std::clamp(
          static_cast<unsigned char>(255 * rgb.y),
          static_cast<unsigned char>(0), static_cast<unsigned char>(255));
      const unsigned char b = std::clamp(
          static_cast<unsigned char>(255 * rgb.z),
          static_cast<unsigned char>(0), static_cast<unsigned char>(255));

      //書き込み
      file << r << " " << g << " " << b << std::endl;
    }
  }

  file.close();
}