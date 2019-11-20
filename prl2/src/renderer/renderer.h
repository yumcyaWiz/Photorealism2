#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include <vector>

#include "core/primitive.h"
#include "core/scene.h"
#include "integrator/integrator.h"
#include "renderer/render_config.h"
#include "renderer/render_layer.h"

namespace Prl2 {

//レンダリングを行うクラス
//与えられた設定を元に、シーンのセットアップ、レンダリングを行う
class Renderer {
 public:
  Renderer(){};

  //レンダリングを行い、結果をRenderLayerに格納する
  void render(const Integrator& integrator, const Scene& scene,
              const RenderConfig& config, RenderLayer& layer) const;
};

}  // namespace Prl2

#endif