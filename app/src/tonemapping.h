#ifndef TONE_MAPPING_H
#define TONE_MAPPING_H

#include <cmath>
#include <vector>

// ガンマ補正を行う
void gammaCorrection(int width, int height, const std::vector<float>& rgb_in,
                     float gamma, std::vector<float>& rgb_out) {
  for (int j = 0; j < height; ++j) {
    for (int i = 0; i < width; ++i) {
      const int index = 3 * i + 3 * width * j;

      const float r = rgb_in[index + 0];
      const float g = rgb_in[index + 1];
      const float b = rgb_in[index + 2];

      rgb_out[index + 0] = std::pow(r, 1 / gamma);
      rgb_out[index + 1] = std::pow(g, 1 / gamma);
      rgb_out[index + 2] = std::pow(b, 1 / gamma);
    }
  }
}

// Linear Tone Mapping
void linearToneMapping(int width, int height, const std::vector<float>& rgb_in,
                       std::vector<float>& rgb_out) {
  for (int j = 0; j < height; ++j) {
    for (int i = 0; i < width; ++i) {
      const int index = 3 * i + 3 * width * j;

      const float r = rgb_in[index + 0];
      const float g = rgb_in[index + 1];
      const float b = rgb_in[index + 2];

      rgb_out[index + 0] = r;
      rgb_out[index + 1] = g;
      rgb_out[index + 2] = b;
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

      rgb_out[index + 0] = r / (1 + r);
      rgb_out[index + 1] = g / (1 + g);
      rgb_out[index + 2] = b / (1 + b);
    }
  }
}

#endif