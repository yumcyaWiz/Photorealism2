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
      // ガンマ補正も同時に行う
      const RGB rgb = spd.toRGB();
      const unsigned int r = std::clamp(
          static_cast<unsigned int>(255 * std::pow(rgb.x, 1 / 2.2)),
          static_cast<unsigned int>(0), static_cast<unsigned int>(255));
      const unsigned int g = std::clamp(
          static_cast<unsigned int>(255 * std::pow(rgb.y, 1 / 2.2)),
          static_cast<unsigned int>(0), static_cast<unsigned int>(255));
      const unsigned int b = std::clamp(
          static_cast<unsigned int>(255 * std::pow(rgb.z, 1 / 2.2)),
          static_cast<unsigned int>(0), static_cast<unsigned int>(255));

      //書き込み
      file << r << " " << g << " " << b << std::endl;
    }
  }

  file.close();
}