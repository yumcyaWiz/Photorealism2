#include <string>

#include "gui.h"
#include "imgui.h"

void GUI::drawSettings(Render& render) const {
  bool refresh_render = false;
  bool refresh_config = false;

  ImGui::Begin("Settings");
  {
    static int size[2] = {render.config.width, render.config.height};
    refresh_config |= ImGui::InputInt2("Image Size", size);

    static int samples = render.config.samples;
    refresh_config |= ImGui::InputInt("Samples", &samples);

    refresh_render |= ImGui::Button("Render");
  }
  ImGui::End();

  if (refresh_render) {
    render.requestRender();
  }
}

void GUI::drawRenderLayer(const Render& render) const {
  ImGui::Begin("RenderLayer");
  {
    // Layer選択ボタン
    static int e = 0;
    ImGui::RadioButton("Render", &e, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Normal", &e, 1);
    ImGui::SameLine();
    ImGui::RadioButton("Position", &e, 2);
    ImGui::SameLine();
    ImGui::RadioButton("Depth", &e, 3);

    // テクスチャの生成
    ImTextureID id;
    if (e == 0) {
      makeTextureFromLayer(render_texture_id, render.layer.render_sRGB);
      id = (ImTextureID)(intptr_t)(render_texture_id);
    } else if (e == 1) {
      makeTextureFromLayer(normal_texture_id, render.layer.normal_sRGB);
      id = (ImTextureID)(intptr_t)(normal_texture_id);
    } else if (e == 2) {
      makeTextureFromLayer(position_texture_id, render.layer.position_sRGB);
      id = (ImTextureID)(intptr_t)(position_texture_id);
    } else if (e == 3) {
      makeTextureFromLayer(depth_texture_id, render.layer.depth_sRGB);
      id = (ImTextureID)(intptr_t)(depth_texture_id);
    }

    // テクスチャの表示
    ImGui::Image(id, ImVec2(width, height));
  }
  ImGui::End();
}

void GUI::makeTextureFromLayer(GLuint texture_id,
                               const std::vector<float>& rgb) const {
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT,
               rgb.data());
  glBindTexture(GL_TEXTURE_2D, 0);
}