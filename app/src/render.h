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
      : cancel_render(false), refresh_render(false) {
    initScene();
    renderer.loadConfig(config);
  };

  // シーンを初期化する
  void initScene();

  // レンダリングする
  void requestRender() {
    // フラグを立てる
    if (refresh_render) {
      cancel_render = true;
    }
    refresh_render = true;
  };

  // レンダリングをキャンセルする
  void cancelRender() { cancel_render = true; };

  // レンダリングしているかどうか
  bool isRendering() const {
    return !refresh_render;
  };

  // レンダリングループ
  // 別スレッドでレンダリングが行われる
  std::thread render();

 private:
  std::atomic<bool> cancel_render;  // レンダラーのキャンセルフラグ
  std::atomic<bool> refresh_render;  // レンダラーの再レンダリングフラグ
};

#endif