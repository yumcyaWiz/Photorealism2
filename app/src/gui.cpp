#include <string>

#include "gui.h"
#include "imgui.h"

#include "core/vec3.h"

void GUI::drawRenderSettings(Render& render) const {
  bool refresh_render = false;

  ImGui::Begin("Render Settings");
  {
    static int size[2] = {render.renderer.config.width,
                          render.renderer.config.height};
    if (ImGui::InputInt2("Image Size", size)) {
      render.renderer.setImageSize(size[0], size[1]);
    }

    static int samples = render.renderer.config.samples;
    if (ImGui::InputInt("Samples", &samples)) {
      render.renderer.setSamples(samples);
    }

    refresh_render |= ImGui::Button("Render");
  }
  ImGui::End();

  if (refresh_render) {
    render.requestRender();
  }
}

void GUI::drawRenderLayer(const Render& render) const {
  ImGui::Begin("Render");
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
    const int width = render.renderer.config.width;
    const int height = render.renderer.config.height;
    if (e == 0) {
      makeTextureFromLayer(render_texture_id, width, height,
                           render.layer.render_sRGB);
      id = (ImTextureID)(intptr_t)(render_texture_id);
    } else if (e == 1) {
      makeTextureFromLayer(normal_texture_id, width, height,
                           render.layer.normal_sRGB);
      id = (ImTextureID)(intptr_t)(normal_texture_id);
    } else if (e == 2) {
      makeTextureFromLayer(position_texture_id, width, height,
                           render.layer.position_sRGB);
      id = (ImTextureID)(intptr_t)(position_texture_id);
    } else if (e == 3) {
      makeTextureFromLayer(depth_texture_id, width, height,
                           render.layer.depth_sRGB);
      id = (ImTextureID)(intptr_t)(depth_texture_id);
    }

    // テクスチャの表示
    ImGui::Image(id, ImVec2(width, height));
  }
  ImGui::End();
}

void GUI::drawFilmSettings(Render& render) const {
  ImGui::Begin("Film");
  {
    static float film_length[2] = {render.renderer.config.width_length,
                                   render.renderer.config.height_length};
    if (ImGui::InputFloat2("Film Length", film_length)) {
      render.renderer.setFilmLength(film_length[0], film_length[1]);
    }
  }
  ImGui::End();
}

void GUI::drawCameraSettings(Render& render) const {
  ImGui::Begin("Camera");
  {
    static float camera_position[3] = {
        render.renderer.config.camera_position.x(),
        render.renderer.config.camera_position.y(),
        render.renderer.config.camera_position.z()};
    if (ImGui::InputFloat3("Camera Position", camera_position)) {
      render.renderer.setCameraLookAt(
          Prl2::Vec3(camera_position[0], camera_position[1],
                     camera_position[2]),
          Prl2::Vec3(0, 0, 0));
    }
  }
  ImGui::End();
}

void GUI::makeTextureFromLayer(GLuint texture_id, int width, int height,
                               const std::vector<float>& rgb) const {
  if (rgb.size() == 3 * width * height) {
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT,
                 rgb.data());
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}