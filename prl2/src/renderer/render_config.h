#ifndef RENDER_CONFIG_H
#define RENDER_CONFIG_H

#include <string>

#include "core/type.h"

namespace Prl2 {

//レンダリングの設定を表すクラス
struct RenderConfig {
  // Film
  int width;           //画像の横幅[px]
  int height;          //画像の縦幅[px]
  Real width_length;   //フィルムの横幅[m]
  Real height_length;  //フィルムの縦幅[m]
};

}  // namespace Prl2
#endif