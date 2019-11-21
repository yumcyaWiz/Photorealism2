#ifndef GUI_H
#define GUI_H

#include "glad/glad.h"

#include "GLFW/glfw3.h"

#include "render.h"
#include "renderer/render_layer.h"

class GUI {
 public:
  GUI(const Prl2::RenderConfig& config)
      : width(config.width), height(config.height) {
    // Textureの用意
    glGenTextures(1, &render_texture_id);
    glGenTextures(1, &normal_texture_id);
    glGenTextures(1, &position_texture_id);
    glGenTextures(1, &depth_texture_id);
  };

  // RenderLayerを表示
  void drawRenderLayer(const Render& render) const;

 private:
  int width;
  int height;
  GLuint render_texture_id;    // Render Texture
  GLuint normal_texture_id;    // Normal Texture
  GLuint position_texture_id;  // Position Texture
  GLuint depth_texture_id;     // Depth Texture

  void makeTextureFromRGB(GLuint texture_id, const float* rgb) const;
};

#endif