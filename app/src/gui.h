#ifndef GUI_H
#define GUI_H

#include "GLFW/glfw3.h"

class GUI {
 public:
  GUI() {
    // Textureの用意
    glGenTextures(1, &render_texture);
    glGenTextures(1, &normal_texture);
    glGenTextures(1, &position_texture);
    glGenTextures(1, &depth_texture);
  };

  // RenderLayerを表示
  void drawRenderLayer() const;

 private:
  GLuint render_texture;    // Render Texture
  GLuint normal_texture;    // Normal Texture
  GLuint position_texture;  // Position Texture
  GLuint depth_texture;     // Depth Texture
};

#endif