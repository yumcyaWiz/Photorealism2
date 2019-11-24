#ifndef RENDER_CONFIG_H
#define RENDER_CONFIG_H

#include <atomic>
#include <string>

#include "core/type.h"
#include "core/vec3.h"

namespace Prl2 {

// Layerの種類
enum class LayerType { Render, Normal, Position, Depth, Sample };

// Imageの種類
enum class ImageType { PPM, PNG, EXR };

// Tone Mappingの種類
enum class ToneMappingType { Linear, Reinhard };

// レンダリングの設定を表すクラス
// 画像のサイズ、サンプル数、カメラの種類、シーンファイルの種類などを設定する
struct RenderConfig {
  RenderConfig(){};

  // Output
  LayerType layer_type = LayerType::Render;  // 出力レイヤーの種類
  ImageType image_type = ImageType::PPM;     // 出力画像形式

  // Post Process
  Real exposure = 1.0;                // 露光
  Real gamma = 2.2;                   // ガンマ値
  ToneMappingType tone_mapping_type;  // Tone Mappingの種類
  Real mapping_factor =
      1;  // https://docs.substance3d.com/spdoc/tone-mapping-162005358.html

  // Film
  int width = 512;               //画像の横幅[px]
  int height = 512;              //画像の縦幅[px]
  Real width_length = 0.0251f;   //フィルムの横幅[m]
  Real height_length = 0.0251f;  //フィルムの縦幅[m]

  // Camera
  std::string camera_type;               //カメラの種類
  Vec3 camera_position = Vec3(0, 1, 3);  // カメラの位置
  Vec3 camera_lookat = Vec3(0, 0, 0);    // カメラの注目位置
  Real fov = 90.0f;                      // 対角FOV

  // Scene
  std::string scene_file;  //シーンファイル

  // Sky
  std::string sky_type;  // 空の種類

  // Sampler
  std::string sampler_type;  // Samplerの種類

  // Integrator
  std::string integrator_type;  // Integratorの種類

  // Renderer
  int samples = 10;  //サンプル数
};

}  // namespace Prl2
#endif