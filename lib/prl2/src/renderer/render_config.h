#ifndef RENDER_CONFIG_H
#define RENDER_CONFIG_H

#include <atomic>
#include <string>

#include "core/type.h"
#include "core/vec3.h"

namespace Prl2 {

// Layerの種類
enum class LayerType { Render, Albedo, Normal, Position, Depth, Sample };

// Imageの種類
enum class ImageType { PPM, PNG, EXR, HDR, PFM };

// Tone Mappingの種類
enum class ToneMappingType { Linear, Reinhard };

// 空の種類
enum class SkyType { Uniform, Hosek, IBL };

// Cameraの種類
enum class CameraType { Pinhole, Environment };

// レンダリングの設定を表すクラス
// 画像のサイズ、サンプル数、カメラの種類、シーンファイルの種類などを設定する
struct RenderConfig {
  RenderConfig(){};
  // Render
  int render_tiles_x = 16;  // X方向のレンダリングタイルの数
  int render_tiles_y = 16;  // Y方向のレンダリングタイルの数

  bool render_realtime = false;  // 画面全体を描画してからサンプルを蓄積するか

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
  CameraType camera_type = CameraType::Pinhole;  // カメラの種類
  Vec3 camera_position = Vec3(0, 1, 3);          // カメラの位置
  Vec3 camera_lookat = Vec3(0, 0, 0);            // カメラの注目位置

  // Pinhole Camera
  Real camera_pinhole_fov = 90.0f;  // 対角FOV

  // Scene
  std::string scene_file;  //シーンファイル

  // Sky
  SkyType sky_type = SkyType::Hosek;  // 空の種類

  // Uniform Sky
  Vec3 uniform_sky_color = Vec3(0, 0, 0);  // 空の色(UniformSky)

  // Hosek Sky
  Vec3 hosek_sky_sun_direciton = Vec3(0, 0, 1);  // 太陽の方向(HosekSky)
  Real hosek_sky_turbidity = 4;                  // Turbidity(HosekSky)
  Vec3 hosek_sky_albedo = Vec3(1, 1, 1);         // Albedo(HosekSky)

  // IBL Sky
  std::string ibl_sky_filename = "PaperMill_E_3k.hdr";  // IBLのファイル名

  // Sampler
  std::string sampler_type;  // Samplerの種類

  // Integrator
  std::string integrator_type;  // Integratorの種類

  // Renderer
  int samples = 10;  //サンプル数
};

}  // namespace Prl2
#endif