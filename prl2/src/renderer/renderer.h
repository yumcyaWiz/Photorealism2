#ifndef RENDERER_H
#define RENDERER_H

#include <memory>

#include "core/scene.h"
#include "renderer/render_config.h"

namespace Prl2 {

//レンダリングを行うクラス
//与えられた設定を元に、シーンのセットアップ、レンダリングを行う
class Renderer {
 public:
  Renderer(){};

  //与えられたシーンと設定からレンダリングを行い、結果をRenderLayerに格納する
  void render(const Scene& scene, const RenderConfig& config) const;
};

}  // namespace Prl2

#endif