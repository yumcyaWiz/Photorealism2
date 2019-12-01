#ifndef RENDERER_H
#define RENDERER_H

#include <atomic>
#include <memory>
#include <vector>

#include "core/primitive.h"
#include "integrator/integrator.h"
#include "io/io.h"
#include "renderer/render_config.h"
#include "renderer/render_layer.h"
#include "renderer/scene.h"

namespace Prl2 {

// レンダリングを行うクラス
// 与えられた設定を元に、シーンのセットアップ、レンダリングを行う
// 外部へのAPIを提供する
class Renderer {
 public:
  RenderConfig config;  // RenderConfig
  Scene scene;          // Scene

  Renderer(){};
  Renderer(const RenderConfig& _config) : config(_config) {
    loadConfig(_config);
  };

  // RenderConfigをもとにRendererの初期化を行う
  void loadConfig(const RenderConfig& config);

  // レンダリングを行い、結果をRenderLayerに格納する
  void render(const std::atomic<bool>& cancel);

  bool getRenderRealtime() const;
  void setRenderRealtime(bool realtime);

  // レンダリングの進捗を入手する
  Real getRenderProgress() const;

  // レンダリングに要した時間を入手する
  int getRenderingTime() const;

  // Render Settings
  // 出力サイズを入手する
  void getImageSize(int& sx, int& sy) const;
  // 出力サイズを変更する
  void setImageSize(unsigned int width, unsigned int height);

  // サンプル数を入手する
  unsigned int getSamples() const;
  // サンプル数を設定する
  void setSamples(unsigned int samples);

  // レンダリングタイル数を入手する
  void getRenderTiles(int& x, int& y) const;
  // レンダリングタイル数をセットする
  void setRenderTiles(int x, int y);

  // 出力レイヤーを入手する
  LayerType getOutputLayer() const;
  // 出力レイヤーを指定する
  void setOutputLayer(const LayerType& _layer_type);

  // 出力画像形式を入手する
  ImageType getImageType() const;
  // 出力画像形式を指定する
  void setImageType(const ImageType& _image_type);

  // Post Processing
  // 露光を入手
  Real getExposure() const;
  // 露光を設定
  void setExposure(const Real& exposure);

  // ガンマ値を入手
  Real getGamma() const;
  // ガンマ値を設定
  void setGamma(const Real& gamma);

  // Tone Mappingの種類を入手
  ToneMappingType getToneMappingType() const;
  // Tone Mappingの種類を指定
  void setToneMappingType(const ToneMappingType& _tone_mapping_type);

  // Tone Mappingの種類を入手
  Real getMappingFactor() const;
  // Mapping Factorを指定
  void setMappingFactor(const Real& _mapping_factor);

  // Film
  // フィルムの長さを入手する
  void getFilmLength(Real& lx, Real& ly) const;
  // フィルムの長さを設定する
  void setFilmLength(const Real& width_length, const Real& height_length);

  // Camera
  // カメラの位置、向きを入手する
  void getCameraLookAt(Vec3& pos, Vec3& lookat) const;
  // カメラの位置、向きを設定する
  void setCameraLookAt(const Vec3& pos, const Vec3& lookat);
  // カメラを移動する
  void moveCamera(const Vec3& pos_diff);
  // カメラを回転する
  void rotateCamera(const Vec3& r);
  // ConfigからCameraを初期化する
  void commitCamera();

  // Sky
  // 空の種類を入手する
  SkyType getSkyType() const;
  // 空の種類を設定する
  void setSkyType(const SkyType& sky_type);
  // Configから空を初期化し、シーンにセットする
  void commitSky();

  // Uniform Sky
  Vec3 getUniformSkyColor() const;
  void setUniformSkyColor(const Vec3& color);

  // Hosek Sky
  Vec3 getHosekSkySunDirection() const;
  Real getHosekSkyTurbidity() const;
  Vec3 getHosekSkyAlbedo() const;
  void setHosekSkySunDirection(const Vec3& sun_direction);
  void setHosekSkyTurbidity(const Real& turbidity);
  void setHosekSkyAlbedo(const Vec3& albedo);

  // LayerをsRGBとして入手
  void getLayersRGB(std::vector<float>& rgb) const;

  // Layerを画像として保存
  void saveLayer(const std::string& filename) const;

 private:
  RenderLayer layer;                       // RenderLayer
  std::shared_ptr<Sampler> sampler;        // Sampler
  std::shared_ptr<Integrator> integrator;  // Integrator

  std::atomic<int> num_rendered_pixels;  // レンダリング済みのピクセル数
  int rendering_time;  // レンダリングにかかった時間[ms]

  // Render LayerをsRGBとして入手
  void getRendersRGB(std::vector<float>& rgb) const;

  // Normal LayerをsRGBとして入手
  void getNormalsRGB(std::vector<float>& rgb) const;

  // Position LayerをsRGBとして入手
  void getPositionsRGB(std::vector<float>& rgb) const;

  // Depth LayerをsRGBとして入手
  void getDepthsRGB(std::vector<float>& rgb) const;

  // Sampler LayerをsRGBとして入手
  void getSamplesRGB(std::vector<float>& rgb) const;
};

}  // namespace Prl2

#endif