#ifndef RENDERER_H
#define RENDERER_H

#include <memory>

namespace Prl2 {

//レンダリングを行うクラス
//与えられた設定を元に、シーンのセットアップ、レンダリングを行う
class Renderer {
 public:
  Renderer(){};

  void render() const;
};

}  // namespace Prl2

#endif