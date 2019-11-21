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
  void render() {
    std::thread rendering_thread([&] {
      while (true) {
        if (refreshRender) {
          const auto start_time = std::chrono::system_clock::now();
          renderer.render(layer);

          if (cancelRender) {
            cancelRender = false;
          } else {
            std::cout << "Rendering Finished in "
                      << std::chrono::duration_cast<std::chrono::milliseconds>(
                             std::chrono::system_clock::now() - start_time)
                             .count()
                      << "ms" << std::endl;
            refreshRender = false;
          }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
    });
  };

 private:
  Prl2::RenderConfig config;  // RenderConfig
  Prl2::RenderLayer layer;    // RenderLayer
  Prl2::Renderer renderer;    // Renderer

  std::atomic<bool> cancelRender;  // レンダラーのキャンセルフラグ
  std::atomic<bool> refreshRender;  // レンダラーの再レンダリングフラグ
};

#endif