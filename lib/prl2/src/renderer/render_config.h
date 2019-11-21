#ifndef RENDER_CONFIG_H
#define RENDER_CONFIG_H

#include <atomic>
#include <string>

#include "core/type.h"
#include "core/vec3.h"

namespace Prl2 {

// レンダリングの設定を表すクラス
// 画像のサイズ、サンプル数、カメラの種類、シーンファイルの種類などを設定する
struct RenderConfig {
  RenderConfig(){};

  // Film
  int width = 512;               //画像の横幅[px]
  int height = 512;              //画像の縦幅[px]
  Real width_length = 0.0251f;   //フィルムの横幅[m]
  Real height_length = 0.0251f;  //フィルムの縦幅[m]

  // Camera
  std::string camera_type;                //カメラの種類
  Vec3 camera_position = Vec3(0, 0, 0);   // カメラの位置
  Vec3 camera_lookat = Vec3(0, 0, -100);  // カメラの注目位置
  Real fov = 90.0f;                       // 対角FOV

  // Scene
  std::string scene_file;  //シーンファイル

  // Sampler
  std::string sampler_type;  // Samplerの種類

  // Integrator
  std::string integrator_type;  // Integratorの種類

  // Renderer
  int samples = 1;  //サンプル数
};

}  // namespace Prl2
#endif