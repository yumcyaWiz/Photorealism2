#include "sky/ibl_sky.h"

#include "stb_image.h"

namespace Prl2 {

IBLSky::IBLSky(const std::string& filename) {
  // HDR画像の読み込み
  int c;
  float* image = stbi_loadf(filename.c_str(), &width, &height, &c, 3);

  // 配列の初期化
  pixels = new SPD[width * height];
  for (int j = 0; j < height; ++j) {
    for (int i = 0; i < width; ++i) {
      const int index = 3 * i + 3 * width * j;
      const float r = image[index + 0];
      const float g = image[index + 1];
      const float b = image[index + 2];
      pixels[i + width * j] = RGB2Spectrum(RGB(r, g, b));
    }
  }
}

IBLSky::~IBLSky() { delete[] pixels; }

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

  return pixels[i + width * j].sample(ray.lambda);
}

}  // namespace Prl2