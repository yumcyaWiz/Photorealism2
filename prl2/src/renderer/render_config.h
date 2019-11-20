#ifndef RENDER_CONFIG_H
#define RENDER_CONFIG_H

#include <atomic>
#include <string>

#include "core/type.h"

namespace Prl2 {

// レンダリングの設定を表すクラス
// 画像のサイズ、サンプル数、カメラの種類、シーンファイルの種類などを設定する
struct RenderConfig {
  RenderConfig() {
    // Default Settings
    width = 512;
    height = 512;
    width_length = 0.0251f;
    height_length = 0.0251f;

    samples = 1;
  };

  // Film
  int width;           //画像の横幅[px]
  int height;          //画像の縦幅[px]
  Real width_length;   //フィルムの横幅[m]
  Real height_length;  //フィルムの縦幅[m]

  // Camera
  std::string camera_type;  //カメラの種類

  // Scene
  std::string scene_file;  //シーンファイル

  // Integrator
  std::string integrator_type;  // Integratorの種類

  // Renderer
  int samples;  //サンプル数
};

}  // namespace Prl2
#endif