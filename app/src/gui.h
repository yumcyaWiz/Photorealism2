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
  void drawRenderSettings(Render& render) const;

  // RenderLayerを表示
  void drawRenderLayer(const Render& render);

  // FilmSettingsを表示
  void drawFilmSettings(Render& render) const;

  // CameraSettingsを表示
  void drawCameraSettings(Render& render) const;

  // ToneMappingUIを表示
  void drawToneMappingUI();

 private:
  GLuint render_texture_id;  // Render Texture

  Prl2::LayerType render_layer_type;  // Renderに表示するレイヤー

  float gamma = 2.2;     // ガンマ値
  float exposure = 2.2;  // 露光値
  Prl2::ToneMappingType tone_mapping_type = Prl2::ToneMappingType::Linear;

  // LayerからTextureを生成
  void makeTextureFromLayer(GLuint texture_id, int width, int height,
                            const std::vector<float>& rgb) const;
};

#endif