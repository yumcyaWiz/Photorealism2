#include "renderer/render-layer.h"

namespace Prl2 {

RenderLayer::RenderLayer(const RenderConfig& config) {
  render_sRGB.resize(3 * config.width * config.height);
  denoised_sRGB.resize(3 * config.width * config.height);
  albedo_sRGB.resize(3 * config.width * config.height);
  normal_sRGB.resize(3 * config.width * config.height);
  depth_sRGB.resize(3 * config.width * config.height);
  position_sRGB.resize(3 * config.width * config.height);
  samples.resize(config.width * config.height);
  sample_sRGB.resize(3 * config.width * config.height);
}

void RenderLayer::resize(int width, int height) {
  render_sRGB.resize(3 * width * height);
  denoised_sRGB.resize(3 * width * height);
  albedo_sRGB.resize(3 * width * height);
  normal_sRGB.resize(3 * width * height);
  depth_sRGB.resize(3 * width * height);
  position_sRGB.resize(3 * width * height);
  samples.resize(width * height);
  sample_sRGB.resize(3 * width * height);
}

void RenderLayer::clear() {
  std::fill(render_sRGB.begin(), render_sRGB.end(), 0);
  std::fill(denoised_sRGB.begin(), denoised_sRGB.end(), 0);
  std::fill(albedo_sRGB.begin(), albedo_sRGB.end(), 0);
  std::fill(normal_sRGB.begin(), normal_sRGB.end(), 0);
  std::fill(depth_sRGB.begin(), depth_sRGB.end(), 0);
  std::fill(position_sRGB.begin(), position_sRGB.end(), 0);
  std::fill(samples.begin(), samples.end(), 1);
  std::fill(sample_sRGB.begin(), sample_sRGB.end(), 0);
}

void RenderLayer::clearPixel(int i, int j, int width, int height) {
  render_sRGB[3 * i + 3 * width * j] = 0;
  render_sRGB[3 * i + 3 * width * j + 1] = 0;
  render_sRGB[3 * i + 3 * width * j + 2] = 0;

  denoised_sRGB[3 * i + 3 * width * j] = 0;
  denoised_sRGB[3 * i + 3 * width * j + 1] = 0;
  denoised_sRGB[3 * i + 3 * width * j + 2] = 0;

  albedo_sRGB[3 * i + 3 * width * j] = 0;
  albedo_sRGB[3 * i + 3 * width * j + 1] = 0;
  albedo_sRGB[3 * i + 3 * width * j + 2] = 0;

  normal_sRGB[3 * i + 3 * width * j] = 0;
  normal_sRGB[3 * i + 3 * width * j + 1] = 0;
  normal_sRGB[3 * i + 3 * width * j + 2] = 0;

  depth_sRGB[3 * i + 3 * width * j] = 0;
  depth_sRGB[3 * i + 3 * width * j + 1] = 0;
  depth_sRGB[3 * i + 3 * width * j + 2] = 0;

  position_sRGB[3 * i + 3 * width * j] = 0;
  position_sRGB[3 * i + 3 * width * j + 1] = 0;
  position_sRGB[3 * i + 3 * width * j + 2] = 0;

  samples[i + width * j] = 1;  // 0割りを防ぐためにサンプル数は1で初期化する

  sample_sRGB[3 * i + 3 * width * j] = 0;
  sample_sRGB[3 * i + 3 * width * j + 1] = 0;
  sample_sRGB[3 * i + 3 * width * j + 2] = 0;
}

}  // namespace Prl2