#ifndef _APP_RENDER_H
#define _APP_RENDER_H

#include <atomic>

#include "renderer/renderer.h"

// レンダラー周りのことをやる
class Render {
 public:
  Render(const Prl2::RenderConfig& config) {
    renderer.loadConfig(config);
    initScene(config);
  };

  // シーンを初期化する
  void initScene(const Prl2::RenderConfig& config);

 private:
  Prl2::Renderer renderer;  // Renderer

  std::atomic<bool> cancelRender;  // レンダラーのキャンセルフラグ
  std::atomic<bool> refreshRender;  // レンダラーの再レンダリングフラグ
};

#endif