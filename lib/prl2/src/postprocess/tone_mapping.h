#ifndef _PRL2_TONE_MAPPING_H
#define _PRL2_TONE_MAPPING_H

#include <cmath>
#include <vector>

namespace Prl2 {

// Linear Tone Mapping
void linearToneMapping(int width, int height, const std::vector<float>& xyz_in,
                       std::vector<float>& xyz_out) {
  for (int j = 0; j < height; ++j) {
    for (int i = 0; i < width; ++i) {
      const int index = 3 * i + 3 * width * j;

      const float x = xyz_in[index + 0];
      const float y = xyz_in[index + 1];
      const float z = xyz_in[index + 2];

      xyz_out[index + 0] = x;
      xyz_out[index + 1] = y;
      xyz_out[index + 2] = z;
    }
  }
}

// Reinhard Tone Mapping
void reinHardToneMapping(int width, int height,
                         const std::vector<float>& rgb_in,
                         std::vector<float>& rgb_out) {
  for (int j = 0; j < height; ++j) {
    for (int i = 0; i < width; ++i) {
      const int index = 3 * i + 3 * width * j;

      const float r = rgb_in[index + 0];
      const float g = rgb_in[index + 1];
      const float b = rgb_in[index + 2];

      const float intensity = std::sqrt(r * r + g * g + b * b);

      rgb_out[index + 0] = r * intensity / (1 + intensity);
      rgb_out[index + 1] = g * intensity / (1 + intensity);
      rgb_out[index + 2] = b * intensity / (1 + intensity);
    }
  }
}

}  // namespace Prl2

#endif