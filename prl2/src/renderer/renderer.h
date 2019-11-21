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
  // RenderConfigをもとにRendererの初期化を行う
  Renderer(const RenderConfig& config);

  //レンダリングを行い、結果をRenderLayerに格納する
  void render(RenderLayer& layer) const;

 private:
  const RenderConfig config;               // RenderConfig
  std::shared_ptr<Sampler> sampler;        // Sampler
  std::shared_ptr<Integrator> integrator;  // Integrator
  Scene scene;                             // Scene
};

}  // namespace Prl2

#endif