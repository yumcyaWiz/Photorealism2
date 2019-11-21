#include "gui.h"
#include "imgui.h"

void GUI::drawRenderLayer() const {
  ImGui::Begin("RenderLayer");
  {
    ImTextureID id = (ImTextureID)(intptr_t)(normal_texture_id);
    ImGui::Image(id, ImVec2(width, height));
  }
  ImGui::End();
}

void GUI::makeTextureFromRGB(GLuint texture_id, int width, int height,
                             const std::vector<float>& rgb) {
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT,
               rgb.data());
}