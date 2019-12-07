#ifndef GUI_H
#define GUI_H

#include <vector>

#include "glad/glad.h"

#include "GLFW/glfw3.h"

#include "render.h"

class GUI {
 public:
  GUI() {
    // Textureの用意
    glGenTextures(1, &render_texture_id);
  };

  // RenderSettingsを表示
  void drawRenderSettings(Render& render);

  // RenderLayerを表示
  void drawRenderLayer(Render& render);

  // CameraSettingsを表示
  void drawCameraSettings(Render& render) const;

  // ToneMappingUIを表示
  void drawToneMappingUI(Render& render);

  // SkyUIを表示
  void drawSkyUI(Render& render) const;

 private:
  GLuint render_texture_id;  // Render Texture

  bool auto_render = true;  // パラメーター変化時に自動で再レンダリングするか
  bool update_texture = false;  // Textureの更新フラグ

  // LayerからTextureを生成
  void makeTextureFromLayer(GLuint texture_id, int width, int height,
                            const std::vector<float>& rgb) const;
};

#endif