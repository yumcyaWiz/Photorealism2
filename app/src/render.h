#ifndef _APP_RENDER_H
#define _APP_RENDER_H

#include <atomic>
#include <chrono>
#include <thread>

#include "renderer/renderer.h"

// レンダラー周りのことをやる
class Render {
 public:
  Render(const Prl2::RenderConfig& config) {
    layer.resize(config.width, config.height);
    renderer.loadConfig(config);
    initScene(config);
  };

  // シーンを初期化する
  void initScene(const Prl2::RenderConfig& config);

  // レンダリングする
  void requestRender() {
    if (refreshRender) {
      cancelRender = true;
    }
    refreshRender = true;
  };

  // レンダリングループ
  // 別スレッドでレンダリングが行われる
  std::thread render();

  // Render Layerの取得
  const Prl2::Real* getRenderLayer() const { return layer.render_sRGB.data(); };
  const Prl2::Real* getNormalLayer() const { return layer.normal_sRGB.data(); };
  const Prl2::Real* getPositionLayer() const {
    return layer.position_sRGB.data();
  };
  const Prl2::Real* getDepthLayer() const { return layer.depth_sRGB.data(); };

 private:
  Prl2::RenderConfig config;  // RenderConfig
  Prl2::RenderLayer layer;    // RenderLayer
  Prl2::Renderer renderer;    // Renderer

  std::atomic<bool> cancelRender;  // レンダラーのキャンセルフラグ
  std::atomic<bool> refreshRender;  // レンダラーの再レンダリングフラグ
};

#endif