#ifndef GUI_H
#define GUI_H

#include <vector>

#include "glad/glad.h"

#include "GLFW/glfw3.h"

#include "render.h"
#include "renderer/render_layer.h"

class GUI {
 public:
  GUI() {
    // Textureの用意
    glGenTextures(1, &render_texture_id);
    glGenTextures(1, &normal_texture_id);
    glGenTextures(1, &position_texture_id);
    glGenTextures(1, &depth_texture_id);
  };

  // RenderSettingsを表示
  void drawRenderSettings(Render& render) const;

  // RenderLayerを表示
  void drawRenderLayer(const Render& render) const;

  // FilmSettingsを表示
  void drawFilmSettings(Render& render) const;

 private:
  GLuint render_texture_id;    // Render Texture
  GLuint normal_texture_id;    // Normal Texture
  GLuint position_texture_id;  // Position Texture
  GLuint depth_texture_id;     // Depth Texture

  void makeTextureFromLayer(GLuint texture_id, int width, int height,
                            const std::vector<float>& rgb) const;
};

#endif