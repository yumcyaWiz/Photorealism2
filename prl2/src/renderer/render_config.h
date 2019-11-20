#ifndef RENDER_CONFIG_H
#define RENDER_CONFIG_H

#include <atomic>
#include <string>

#include "core/type.h"

namespace Prl2 {

//レンダリングの設定を表すクラス
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

  // Renderer
  int samples;                       //サンプル数
  std::atomic<int> current_samples;  // 現在のサンプル数
};

}  // namespace Prl2
#endif