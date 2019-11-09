#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include <map>
#include <string>

#include "core/primitive.h"
#include "core/scene.h"
#include "material/material.h"
#include "renderer/render_config.h"
#include "shape/shape.h"
#include "texture/texture.h"

namespace Prl2 {

// シーンファイルからSceneとConfigを作るクラス
class SceneLoader {
 public:
  SceneLoader(){};

  // TomlからSceneとConfigを初期化する
  // Shape, Material, Textureはこの中で生成される
  void loadSceneFromToml(const std::string& filename, RenderConfig& config,
                         Scene& scene);

 private:
  //名前をキーとしてそれぞれのオブジェクトのポインタを格納する
  std::map<std::string, std::shared_ptr<Shape>> shape;         // Shapeの配列
  std::map<std::string, std::shared_ptr<Material>> materials;  // Materialの配列
  std::map<std::string, std::shared_ptr<Texture>> textures;  // Textureの配列
};

}  // namespace Prl2

#endif