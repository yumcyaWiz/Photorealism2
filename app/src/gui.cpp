#include "gui.h"
#include "imgui.h"

void GUI::drawRenderLayer(const Render& render) const {
  ImGui::Begin("RenderLayer");
  {
    // Layer選択ボタン
    static int e = 0;
    ImGui::RadioButton("Render", &e, 0);
    ImGui::RadioButton("Normal", &e, 1);
    ImGui::RadioButton("Position", &e, 2);
    ImGui::RadioButton("Depth", &e, 3);

    // テクスチャの生成
    ImTextureID id;
    if (e == 0) {
      makeTextureFromRGB(render_texture_id, render.getRenderLayer());
      id = (ImTextureID)(intptr_t)(render_texture_id);
    } else if (e == 1) {
      makeTextureFromRGB(normal_texture_id, render.getNormalLayer());
      id = (ImTextureID)(intptr_t)(normal_texture_id);
    } else if (e == 2) {
      makeTextureFromRGB(position_texture_id, render.getPositionLayer());
      id = (ImTextureID)(intptr_t)(position_texture_id);
    } else if (e == 3) {
      makeTextureFromRGB(depth_texture_id, render.getDepthLayer());
      id = (ImTextureID)(intptr_t)(depth_texture_id);
    }

    // テクスチャの表示
    ImGui::Image(id, ImVec2(width, height));
  }
  ImGui::End();
}

void GUI::makeTextureFromRGB(GLuint texture_id, const float* rgb) const {
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT,
               rgb);
}