#ifndef RENDERER_H
#define RENDERER_H

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

  // Render Settings
  // 出力サイズを入手する
  void getImageSize(int& sx, int& sy) const {
    sx = config.width;
    sy = config.height;
  };
  // 出力サイズを変更する
  void setImageSize(unsigned int width, unsigned int height) {
    config.width = width;
    config.height = height;
    layer.resize(width, height);
    scene.camera->film->resize(width, height);
  };

  // サンプル数を入手する
  unsigned int getSamples() const { return config.samples; };
  // サンプル数を設定する
  void setSamples(unsigned int samples) { config.samples = samples; }

  // 出力レイヤーを入手する
  LayerType getOutputLayer() const { return config.layer_type; };
  // 出力レイヤーを指定する
  void setOutputLayer(const LayerType& _layer_type) {
    config.layer_type = _layer_type;
  };

  // 出力画像形式を入手する
  ImageType getImageType() const { return config.image_type; };
  // 出力画像形式を指定する
  void setImageType(const ImageType& _image_type) {
    config.image_type = _image_type;
  };

  // Post Processing
  // 露光を設定
  Real getExposure() const { return config.exposure; };
  void setExposure(const Real& exposure) { config.exposure = exposure; };

  // ガンマ値を入手
  Real getGamma() const { return config.gamma; };
  // ガンマ値を設定
  void setGamma(const Real& gamma) { config.gamma = gamma; };

  // Tone Mappingの種類を入手
  ToneMappingType getToneMappingType() const {
    return config.tone_mapping_type;
  };
  // Tone Mappingの種類を指定
  void setToneMappingType(const ToneMappingType& _tone_mapping_type) {
    config.tone_mapping_type = _tone_mapping_type;
  };

  // Tone Mappingの種類を入手
  Real getMappingFactor() const { return config.mapping_factor; };
  // Mapping Factorを指定
  void setMappingFactor(const Real& _mapping_factor) {
    config.mapping_factor = _mapping_factor;
  };

  // Film
  // フィルムの長さを入手する
  void getFilmLength(Real& lx, Real& ly) const {
    lx = config.width_length;
    ly = config.height_length;
  };
  // フィルムの長さを設定する
  void setFilmLength(const Real& width_length, const Real& height_length) {
    config.width_length = width_length;
    config.height_length = height_length;
    scene.camera->film->resizeLength(width_length, height_length);
  };

  // Camera
  // カメラの位置、向きを入手する
  void getCameraLookAt(Vec3& pos, Vec3& lookat) {
    pos = config.camera_position;
    lookat = config.camera_lookat;
  };
  // カメラの位置、向きを設定する
  void setCameraLookAt(const Vec3& pos, const Vec3& lookat) {
    config.camera_position = pos;
    config.camera_lookat = lookat;
    scene.camera->setLookAt(pos, lookat);
  };

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
  void getLayersRGB(std::vector<float>& rgb) const {
    if (config.layer_type == LayerType::Render) {
      getRendersRGB(rgb);
    } else if (config.layer_type == LayerType::Normal) {
      getNormalsRGB(rgb);
    } else if (config.layer_type == LayerType::Position) {
      getPositionsRGB(rgb);
    } else if (config.layer_type == LayerType::Depth) {
      getDepthsRGB(rgb);
    } else if (config.layer_type == LayerType::Sample) {
      getSamplesRGB(rgb);
    }
  };

  // Layerを画像として保存
  void saveLayer(const std::string& filename) const;

 private:
  RenderLayer layer;                       // RenderLayer
  std::shared_ptr<Sampler> sampler;        // Sampler
  std::shared_ptr<Integrator> integrator;  // Integrator

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