#include "sky/ibl_sky.h"

#include <algorithm>

#include "stb_image.h"

namespace Prl2 {

IBLSky::IBLSky(const std::string& filename) {
  // HDR画像の読み込み
  int c;
  pixels = stbi_loadf(filename.c_str(), &width, &height, &c, 3);

  // 正規化
  Real max = 0;
  for (int j = 0; j < height; ++j) {
    for (int i = 0; i < width; ++i) {
      const Real r = pixels[3 * i + 3 * width * j];
      const Real g = pixels[3 * i + 3 * width * j + 1];
      const Real b = pixels[3 * i + 3 * width * j + 2];
      const Real intensity = std::sqrt(r * r + g * g + b * b);
      if (intensity > max) {
        max = intensity;
      }
    }
  }

  for (int j = 0; j < height; ++j) {
    for (int i = 0; i < width; ++i) {
      pixels[3 * i + 3 * width * j] /= max;
      pixels[3 * i + 3 * width * j + 1] /= max;
      pixels[3 * i + 3 * width * j + 2] /= max;
    }
  }
}

IBLSky::~IBLSky() { stbi_image_free(pixels); }

Real IBLSky::getRadiance(const Ray& ray) const {
  // 球面座標を計算
  Real theta, phi;
  cartesianToSpherical(ray.direction, theta, phi);

  // (u, v)を計算
  const Real u = phi * INV_PI_MUL_2;
  const Real v = theta * INV_PI;

  // (i, j)を計算
  const int i = u * width;
  const int j = v * height;

  // RGBをSPDに変換
  const Real r = pixels[3 * i + 3 * width * j];
  const Real g = pixels[3 * i + 3 * width * j + 1];
  const Real b = pixels[3 * i + 3 * width * j + 2];
  const SPD spd = RGB2Spectrum(RGB(r, g, b));

  return spd.sample(ray.lambda);
}

}  // namespace Prl2