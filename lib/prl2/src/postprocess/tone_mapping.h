#ifndef _PRL2_TONE_MAPPING_H
#define _PRL2_TONE_MAPPING_H

#include <cmath>
#include <vector>

namespace Prl2 {

// 露光調整を行う
void adjustExposure(int width, int height, const std::vector<float>& xyz_in,
                    float exposure, std::vector<float>& xyz_out) {
  // 平均輝度を計算
  float mean = 0;
  for (int j = 0; j < height; ++j) {
    for (int i = 0; i < width; ++i) {
      const int index = 3 * i + 3 * width * j;

      const float y = xyz_in[index + 1];

      mean += y;
    }
  }
  mean /= (width * height);

  for (int j = 0; j < height; ++j) {
    for (int i = 0; i < width; ++i) {
      const int index = 3 * i + 3 * width * j;

      const float x = xyz_in[index + 0];
      const float y = xyz_in[index + 1];
      const float z = xyz_in[index + 2];

      xyz_out[index + 0] = x;
      xyz_out[index + 1] = y * exposure / mean;
      xyz_out[index + 2] = z;
    }
  }
}

// ガンマ補正を行う
void gammaCorrection(int width, int height, const std::vector<float>& xyz_in,
                     float gamma, std::vector<float>& xyz_out) {
  for (int j = 0; j < height; ++j) {
    for (int i = 0; i < width; ++i) {
      const int index = 3 * i + 3 * width * j;

      const float x = xyz_in[index + 0];
      const float y = xyz_in[index + 1];
      const float z = xyz_in[index + 2];

      xyz_out[index + 0] = x;
      xyz_out[index + 1] = y * std::pow(y, 1 / gamma);
      xyz_out[index + 2] = z;
    }
  }
}

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