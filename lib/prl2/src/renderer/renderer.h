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

//レンダリングを行うクラス
//与えられた設定を元に、シーンのセットアップ、レンダリングを行う
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
  // 出力サイズを変更する
  void setImageSize(unsigned int width, unsigned int height) {
    config.width = width;
    config.height = height;
    layer.resize(width, height);
    scene.camera->film->resize(width, height);
  };

  // サンプル数を設定する
  void setSamples(unsigned int samples) { config.samples = samples; }

  // 出力レイヤーを指定する
  void setOutputLayer(const LayerType& _layer_type) {
    config.layer_type = _layer_type;
  };

  // 出力画像形式を指定する
  void setImageType(const ImageType& _image_type) {
    config.image_type = _image_type;
  };

  // Post Processing
  // Tone Mappingの種類を指定
  void setToneMappingType(const ToneMappingType& _tone_mapping_type) {
    config.tone_mapping_type = _tone_mapping_type;
  };

  // Film
  // フィルムの長さを設定する
  void setFilmLength(const Real& width_length, const Real& height_length) {
    config.width_length = width_length;
    config.height_length = height_length;
    scene.camera->film->resizeLength(width_length, height_length);
  };

  // カメラの位置、向きを設定する
  void setCameraLookAt(const Vec3& pos, const Vec3& lookat) {
    config.camera_position = pos;
    config.camera_lookat = lookat;
    scene.camera->setLookAt(pos, lookat);
  };

  // LayerをsRGBとして入手
  void getLayersRGB(const LayerType& layer_type, const ToneMappingType& tm_type,
                    float exposure, float gamma,
                    std::vector<float>& rgb) const {
    if (layer_type == LayerType::Render) {
      getRendersRGB(tm_type, exposure, gamma, rgb);
    } else if (layer_type == LayerType::Normal) {
      getNormalsRGB(rgb);
    } else if (layer_type == LayerType::Position) {
      getPositionsRGB(rgb);
    } else if (layer_type == LayerType::Depth) {
      getDepthsRGB(rgb);
    }
  };

  // Layerを画像として保存
  void saveLayer(const std::string& filename, const LayerType& layer_type,
                 const ToneMappingType& tm_type, float exposure, float gamma,
                 const ImageType& image_type) {
    std::vector<float> image;
    getLayersRGB(layer_type, tm_type, exposure, gamma, image);

    if (image_type == ImageType::PPM) {
      writePPM(filename, config.width, config.height, image);
    } else {
      writePPM(filename, config.width, config.height, image);
    }
  };

 private:
  RenderLayer layer;                       // RenderLayer
  std::shared_ptr<Sampler> sampler;        // Sampler
  std::shared_ptr<Integrator> integrator;  // Integrator

  // Render LayerをsRGBとして入手
  void getRendersRGB(const ToneMappingType& tm_type, float exposure,
                     float gamma, std::vector<float>& rgb) const;

  // Normal LayerをsRGBとして入手
  void getNormalsRGB(std::vector<float>& rgb) const;

  // Position LayerをsRGBとして入手
  void getPositionsRGB(std::vector<float>& rgb) const;

  // Depth LayerをsRGBとして入手
  void getDepthsRGB(std::vector<float>& rgb) const;
};

}  // namespace Prl2

#endif