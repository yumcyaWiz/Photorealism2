#include <string>

#include "imgui.h"

#include "gui.h"

#include "core/vec3.h"
#include "renderer/renderer.h"

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

    static char filename[32];
    ImGui::InputText("Image Filename", filename, 32);

    static int image_type = 0;
    ImGui::RadioButton("PPM", &image_type, 0);
    ImGui::SameLine();
    ImGui::RadioButton("PNG", &image_type, 1);
    ImGui::SameLine();
    ImGui::RadioButton("EXR", &image_type, 2);

    if (ImGui::Button("Save Image")) {
      const int width = render.renderer.config.width;
      const int height = render.renderer.config.height;

      // PPM
      if (image_type == 0) {
        render.renderer.saveLayer(std::string(filename),
                                  Prl2::LayerType::Render,
                                  Prl2::ImageType::PPM);
      }
    }
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
    std::vector<float> image;
    if (e == 0) {
      render.renderer.getLayersRGB(Prl2::LayerType::Render, image);
      makeTextureFromLayer(render_texture_id, width, height, image);
      id = (ImTextureID)(intptr_t)(render_texture_id);
    } else if (e == 1) {
      render.renderer.getLayersRGB(Prl2::LayerType::Normal, image);
      makeTextureFromLayer(normal_texture_id, width, height, image);
      id = (ImTextureID)(intptr_t)(normal_texture_id);
    } else if (e == 2) {
      render.renderer.getLayersRGB(Prl2::LayerType::Position, image);
      makeTextureFromLayer(position_texture_id, width, height, image);
      id = (ImTextureID)(intptr_t)(position_texture_id);
    } else if (e == 3) {
      render.renderer.getLayersRGB(Prl2::LayerType::Depth, image);
      makeTextureFromLayer(depth_texture_id, width, height, image);
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
    static bool lookat_modified = false;
    static float camera_position[3] = {
        render.renderer.config.camera_position.x(),
        render.renderer.config.camera_position.y(),
        render.renderer.config.camera_position.z()};
    lookat_modified |= ImGui::InputFloat3("Camera Position", camera_position);

    static float lookat[3] = {
        render.renderer.config.camera_lookat.x(),
        render.renderer.config.camera_lookat.y(),
        render.renderer.config.camera_lookat.z(),
    };
    lookat_modified |= ImGui::InputFloat3("Lookat", lookat);

    if (lookat_modified) {
      render.renderer.setCameraLookAt(
          Prl2::Vec3(camera_position[0], camera_position[1],
                     camera_position[2]),
          Prl2::Vec3(lookat[0], lookat[1], lookat[2]));
    }
  }

  ImGui::End();
}

void GUI::drawToneMappingUI() {
  ImGui::Begin("Tone Mapping");
  {
    static float _exposure = exposure;
    if (ImGui::InputFloat("Exposure", &_exposure)) {
      exposure = _exposure;
    }

    static int _tone_mapping_function = tone_mapping_function;
    ImGui::RadioButton("Linear", &_tone_mapping_function, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Reinhard", &_tone_mapping_function, 1);
    tone_mapping_function = _tone_mapping_function;

    static float _gamma = gamma;
    if (ImGui::InputFloat("Gamma", &_gamma)) {
      gamma = _gamma;
    }
  }
  ImGui::End();
}

void GUI::makeTextureFromLayer(GLuint texture_id, int width, int height,
                               const std::vector<float>& rgb) const {
  if (rgb.size() != 3 * width * height) return;

  // テクスチャの生成
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT,
               rgb.data());
  glBindTexture(GL_TEXTURE_2D, 0);
}