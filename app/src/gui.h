#ifndef GUI_H
#define GUI_H

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

  // RenderLayerを表示
  void drawRenderLayer() const;

 private:
  int width;
  int height;
  GLuint render_texture_id;    // Render Texture
  GLuint normal_texture_id;    // Normal Texture
  GLuint position_texture_id;  // Position Texture
  GLuint depth_texture_id;     // Depth Texture

  // RenderLayer Textureを更新
  void updateRenderLayerTexture(const Prl2::RenderLayer& layer) {
    makeTextureFromRGB(render_texture_id, width, height, layer.render_sRGB);
    makeTextureFromRGB(normal_texture_id, width, height, layer.normal_sRGB);
    makeTextureFromRGB(position_texture_id, width, height, layer.position_sRGB);
    makeTextureFromRGB(depth_texture_id, width, height, layer.depth_sRGB);
  };

  void makeTextureFromRGB(GLuint texture_id, int width, int height,
                          const std::vector<float>& rgb);
};

#endif