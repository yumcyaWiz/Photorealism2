#ifndef _APP_RENDER_H
#define _APP_RENDER_H

#include <atomic>
#include <chrono>
#include <thread>

#include "renderer/renderer.h"

// レンダラー周りのことをやる
class Render {
 public:
  Prl2::Renderer renderer;  // Renderer

  Render(const Prl2::RenderConfig& config)
      : cancelRender(false), refreshRender(false) {
    initScene();
    renderer.loadConfig(config);
  };

  // シーンを初期化する
  void initScene();

  // レンダリングする
  void requestRender() {
    // フラグを立てる
    if (refreshRender) {
      cancelRender = true;
    }
    refreshRender = true;
  };

  // レンダリングループ
  // 別スレッドでレンダリングが行われる
  std::thread render();

 private:
  std::atomic<bool> cancelRender;  // レンダラーのキャンセルフラグ
  std::atomic<bool> refreshRender;  // レンダラーの再レンダリングフラグ
};

#endif