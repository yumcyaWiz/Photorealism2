#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H

#include <map>
#include <string>

#include "core/primitive.h"
#include "material/material.h"
#include "renderer/render_config.h"
#include "texture/texture.h"

namespace Prl2 {

//シーンを表現するクラス
class SceneGraph {
 public:
  // Configからシーンを初期化する
  SceneGraph(const RenderConfig& config);

 private:
  //名前をキーとしてそれぞれのオブジェクトのポインタを格納する
  std::map<std::string, std::shared_ptr<Primitive>>
      primitives;  //プリミティブの配列
  std::map<std::string, std::shared_ptr<Material>>
      materials;  //マテリアルの配列
  std::map<std::string, std::shared_ptr<Texture>> textures;  //テクスチャの配列
};

}  // namespace Prl2

#endif