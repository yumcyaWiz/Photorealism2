#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include <map>
#include <string>

#include "core/primitive.h"
#include "material/material.h"
#include "renderer/render-config.h"
#include "renderer/scene.h"
#include "shape/shape.h"
#include "texture/texture.h"

namespace Prl2 {

bool loadTriangleMeshFromObj(const std::string& filename);

// シーンファイルを読み込み、Sceneクラスを作成する
class SceneLoader {
 public:
  SceneLoader(){};

 private:
  //名前をキーとしてそれぞれのオブジェクトのポインタを格納する
  std::map<std::string, std::shared_ptr<Shape>> shape;         // Shapeの配列
  std::map<std::string, std::shared_ptr<Material>> materials;  // Materialの配列
  std::map<std::string, std::shared_ptr<Texture>> textures;  // Textureの配列
};

}  // namespace Prl2

#endif