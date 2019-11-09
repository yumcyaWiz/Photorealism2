#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include <vector>

#include "core/primitive.h"
#include "core/scene.h"
#include "renderer/render_config.h"
#include "renderer/render_layer.h"

namespace Prl2 {

//レンダリングを行うクラス
//与えられた設定を元に、シーンのセットアップ、レンダリングを行う
class Renderer {
 public:
  //与えられたRenderConfigからシーンのセットアップを行う
  Renderer(const RenderConfig& _config);

  //レンダリングを行い、結果をRenderLayerに格納する
  void render(RenderLayer& layer) const;

 private:
  const RenderConfig config;
};

}  // namespace Prl2

#endif