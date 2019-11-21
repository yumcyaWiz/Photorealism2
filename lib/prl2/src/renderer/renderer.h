#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include <vector>

#include "core/primitive.h"
#include "integrator/integrator.h"
#include "renderer/render_config.h"
#include "renderer/render_layer.h"
#include "renderer/scene.h"

namespace Prl2 {

//レンダリングを行うクラス
//与えられた設定を元に、シーンのセットアップ、レンダリングを行う
class Renderer {
 public:
  RenderConfig config;  // RenderConfig
  Scene scene;          // Scene

  Renderer(){};
  Renderer(const RenderConfig& _config) : config(_config) {
    loadConfig(_config);
  };

  // RenderConfigをもとにRendererの初期化を行う
  void loadConfig(const RenderConfig& config);

  // 出力サイズを変更する
  void setImageSize(unsigned int width, unsigned int height) {
    config.width = width;
    config.height = height;
    scene.camera->film->resize(width, height);
  };

  // サンプル数を設定する
  void setSamples(unsigned int samples) { config.samples = samples; }

  // フィルムサイズを設定する
  void setFilmSize(const Real& width_length, const Real& height_length) {
    config.width_length = width_length;
    config.height_length = height_length;
    scene.camera->film->resizeLength(width_length, height_length);
  };

  // レンダリングを行い、結果をRenderLayerに格納する
  void render(RenderLayer& layer, const std::atomic<bool>& cancel) const;

 private:
  std::shared_ptr<Sampler> sampler;        // Sampler
  std::shared_ptr<Integrator> integrator;  // Integrator
};

}  // namespace Prl2

#endif