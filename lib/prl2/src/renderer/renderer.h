#ifndef RENDERER_H
#define RENDERER_H

#include <atomic>
#include <memory>
#include <vector>

#include "core/primitive.h"
#include "integrator/integrator.h"
#include "io/io.h"
#include "parallel/parallel.h"
#include "renderer/render-config.h"
#include "renderer/render-layer.h"
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

  // デノイズする
  void denoise();

  // 指定した画素のサンプルパスを生成する
  void generatePath(unsigned int i, unsigned int j,
                    std::vector<Ray>& path) const;

  bool getRenderInteractive() const;
  void setRenderInteractive(bool realtime);

  // レンダリングの進捗を入手する
  Real getRenderProgress() const;

  // レンダリングに要した時間を入手する
  int getRenderingTime() const;

  // Render Settings
  // 出力サイズを入手する
  void getImageSize(unsigned int& sx, unsigned int& sy) const;
  // 出力サイズを変更する
  void setImageSize(unsigned int width, unsigned int height);

  // サンプル数を入手する
  unsigned int getSamples() const;
  // サンプル数を設定する
  void setSamples(unsigned int samples);

  // レンダリングタイル数を入手する
  void getRenderTiles(unsigned int& x, unsigned int& y) const;
  // レンダリングタイル数をセットする
  void setRenderTiles(unsigned int x, unsigned int y);

  // (i, j)のsRGBを入手する
  RGB getsRGB(unsigned int i, unsigned int j) const;
  // (i, j)のNormalを入手する
  Vec3 getNormal(unsigned int i, unsigned int j) const;
  // (i, j)のDepthを入手する
  Real getDepth(unsigned int i, unsigned int j) const;

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
  // カメラのTransform Matrixを入手
  void getCameraMatrix(Mat4& mat) const;
  // カメラの位置、向きを設定する
  void setCameraLookAt(const Vec3& pos, const Vec3& lookat);
  // カメラを移動する
  void moveCamera(const Vec3& pos_diff);
  // カメラを回転する
  void rotateCamera(const Vec3& r);
  // カメラをLookAtを中心とする球面上に配置する
  void setCameraAroundLookAt(const Real& theta, const Real& phi);

  // カメラの種類を入手
  CameraType getCameraType() const;
  // カメラの種類をセット
  void setCameraType(const CameraType& type);

  // Pinhole Camera
  // FOVを入手
  float getPinholeCameraFOV() const;
  void setPinholeCameraFOV(float fov);

  // Thin Lens Camera
  // FOVを入手
  float getThinLensCameraFOV() const;
  // FOVをセット
  void setThinLensCameraFOV(float fov);
  // レンズ半径を入手
  float getThinLensCameraLensRadius() const;
  // レンズ半径をセット
  void setThinLensCameraLensRadius(float radius);
  // ピントの合う位置までの距離を入手
  float getThinLensCameraFocusDistance() const;
  // ピントの合う位置までの距離をセット
  void setThinLensCameraFocusDistance(float distance);

  // FilmにあるSPDを入手する
  SPD getSPD(unsigned int i, unsigned int j) const;

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

  // IBL Sky
  std::string getIBLSkyFilename() const;
  void setIBLSkyFilename(const std::string& filename);

  // Integrator
  IntegratorType getIntegratorType() const;
  void setIntegratorType(const IntegratorType& type);

  // LayerをsRGBとして入手
  void getLayersRGB(std::vector<float>& rgb) const;

  // Layerを画像として保存
  void saveLayer(const std::string& filename) const;

 private:
  RenderLayer layer;                       // RenderLayer
  std::shared_ptr<Sampler> sampler;        // Sampler
  std::shared_ptr<Integrator> integrator;  // Integrator
  Parallel pool;                           // Rendering Thhread Pool

  std::atomic<uint64_t> num_rendered_pixels;  // レンダリング済みのピクセル数
  unsigned int rendering_time;  // レンダリングにかかった時間[ms]

  // (i, j)のレンダリングを行う
  void renderPixel(unsigned int i, unsigned int j, Sampler& sampler);

  // Render LayerをsRGBとして入手
  void getRendersRGB(std::vector<float>& rgb) const;

  // Denoise LayerをsRGBとして入手
  void getDenoisesRGB(std::vector<float>& rgb) const;

  // Albedo LayerをsRGBとして入手
  void getAlbedosRGB(std::vector<float>& rgb) const;

  // Normal LayerをsRGBとして入手
  void getNormalsRGB(std::vector<float>& rgb) const;

  // UV LayerをsRGBとして入手
  void getUVsRGB(std::vector<float>& rgb) const;

  // Position LayerをsRGBとして入手
  void getPositionsRGB(std::vector<float>& rgb) const;

  // Depth LayerをsRGBとして入手
  void getDepthsRGB(std::vector<float>& rgb) const;

  // Sampler LayerをsRGBとして入手
  void getSamplesRGB(std::vector<float>& rgb) const;
};

}  // namespace Prl2

#endif