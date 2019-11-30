#include <string>

#include "imgui.h"

#include "gui.h"

#include "core/vec3.h"
#include "renderer/renderer.h"

void GUI::drawRenderSettings(Render& render) {
  bool refresh_render = false;

  ImGui::Begin("Render Settings");
  {
    int sx, sy;
    render.renderer.getImageSize(sx, sy);
    static int size[2] = {sx, sy};
    if (ImGui::InputInt2("Image Size", size)) {
      render.renderer.setImageSize(size[0], size[1]);
      refresh_render = true;
    }

    static int samples = render.renderer.getSamples();
    if (ImGui::InputInt("Samples", &samples)) {
      render.renderer.setSamples(samples);
      refresh_render = true;
    }

    static int render_tiles[2] = {16, 16};
    if (ImGui::InputInt2("Render Tiles", render_tiles)) {
      render.renderer.setRenderTiles(render_tiles[0], render_tiles[1]);
    }

    if (render.isRendering()) {
      if (ImGui::Button("Cancel")) {
        render.cancelRender();
      }
      ImGui::SameLine();
      ImGui::ProgressBar(render.renderer.getRenderProgress());
    } else {
      if (ImGui::Button("Render")) {
        render.requestRender();
      }
      ImGui::SameLine();
      ImGui::Text((std::to_string(render.renderer.getRenderingTime()) + " [ms]")
                      .c_str());
    }

    static bool _realtime = render.renderer.getRenderRealtime();
    if (ImGui::Checkbox("Realtime", &_realtime)) {
      render.renderer.setRenderRealtime(_realtime);
    }
    ImGui::SameLine();

    static bool _auto_render = auto_render;
    if (ImGui::Checkbox("Auto Refresh", &_auto_render)) {
      auto_render = _auto_render;
    }

    static char filename[32];
    ImGui::InputText("Image Filename", filename, 32);

    static int image_type = 0;
    ImGui::RadioButton("PPM", &image_type, 0);
    ImGui::SameLine();
    ImGui::RadioButton("PNG", &image_type, 1);
    ImGui::SameLine();
    ImGui::RadioButton("EXR", &image_type, 2);
    ImGui::SameLine();
    ImGui::RadioButton("HDR", &image_type, 3);

    if (ImGui::Button("Save Image")) {
      // Set image type
      if (image_type == 0) {
        render.renderer.setImageType(Prl2::ImageType::PPM);
      } else if (image_type == 1) {
        render.renderer.setImageType(Prl2::ImageType::PNG);
      } else if (image_type == 2) {
        render.renderer.setImageType(Prl2::ImageType::EXR);
      } else if (image_type == 3) {
        render.renderer.setImageType(Prl2::ImageType::HDR);
      }
      // Save layer
      render.renderer.saveLayer(std::string(filename));
    }
  }
  ImGui::End();

  if (refresh_render && auto_render) {
    render.requestRender();
  }
}

void GUI::drawRenderLayer(Render& render) const {
  ImGui::Begin("Render");
  {
    // Layer選択ボタン
    static int e = 0;
    bool layer_changed = false;
    layer_changed |= ImGui::RadioButton("Render", &e, 0);
    ImGui::SameLine();
    layer_changed |= ImGui::RadioButton("Normal", &e, 1);
    ImGui::SameLine();
    layer_changed |= ImGui::RadioButton("Position", &e, 2);
    ImGui::SameLine();
    layer_changed |= ImGui::RadioButton("Depth", &e, 3);
    ImGui::SameLine();
    layer_changed |= ImGui::RadioButton("Sample", &e, 4);
    if (layer_changed) {
      if (e == 0) {
        render.renderer.setOutputLayer(Prl2::LayerType::Render);
      } else if (e == 1) {
        render.renderer.setOutputLayer(Prl2::LayerType::Normal);
      } else if (e == 2) {
        render.renderer.setOutputLayer(Prl2::LayerType::Position);
      } else if (e == 3) {
        render.renderer.setOutputLayer(Prl2::LayerType::Depth);
      } else if (e == 4) {
        render.renderer.setOutputLayer(Prl2::LayerType::Sample);
      }
    }

    // テクスチャの生成
    int width, height;
    render.renderer.getImageSize(width, height);
    std::vector<float> image;
    render.renderer.getLayersRGB(image);
    makeTextureFromLayer(render_texture_id, width, height, image);
    ImTextureID id = (ImTextureID)(intptr_t)(render_texture_id);

    // テクスチャの表示
    ImGui::Image(id, ImVec2(width, height));
  }
  ImGui::End();
}

void GUI::drawFilmSettings(Render& render) const {
  bool refresh_render = false;

  ImGui::Begin("Film");
  {
    float lx, ly;
    render.renderer.getFilmLength(lx, ly);
    static float film_length[2] = {lx, ly};
    if (ImGui::InputFloat2("Film Length", film_length)) {
      render.renderer.setFilmLength(film_length[0], film_length[1]);
      refresh_render = true;
    }
  }
  ImGui::End();

  if (refresh_render && auto_render) {
    render.requestRender();
  }
}

void GUI::drawCameraSettings(Render& render) const {
  bool refresh_render = false;

  ImGui::Begin("Camera");
  {
    bool lookat_modified = false;
    Prl2::Vec3 pos, lookat;
    render.renderer.getCameraLookAt(pos, lookat);
    static float camera_position[3] = {pos.x(), pos.y(), pos.z()};
    lookat_modified |= ImGui::InputFloat3("Camera Position", camera_position);

    static float lookat_position[3] = {lookat.x(), lookat.y(), lookat.z()};
    lookat_modified |= ImGui::InputFloat3("Lookat", lookat_position);

    if (lookat_modified) {
      render.renderer.setCameraLookAt(
          Prl2::Vec3(camera_position[0], camera_position[1],
                     camera_position[2]),
          Prl2::Vec3(lookat_position[0], lookat_position[1],
                     lookat_position[2]));
      refresh_render = true;
    }
  }
  ImGui::End();

  if (refresh_render && auto_render) {
    render.requestRender();
  }
}

void GUI::drawToneMappingUI(Render& render) const {
  ImGui::Begin("Tone Mapping");
  {
    static float exposure = render.renderer.getExposure();
    if (ImGui::InputFloat("Exposure", &exposure)) {
      render.renderer.setExposure(exposure);
    }

    static int tm_type = 0;
    bool tm_type_changed = false;
    tm_type_changed |= ImGui::RadioButton("Linear", &tm_type, 0);
    ImGui::SameLine();
    tm_type_changed |= ImGui::RadioButton("Reinhard", &tm_type, 1);
    if (tm_type_changed) {
      if (tm_type == 0) {
        render.renderer.setToneMappingType(Prl2::ToneMappingType::Linear);
      } else if (tm_type == 1) {
        render.renderer.setToneMappingType(Prl2::ToneMappingType::Reinhard);
      }
    }

    static float mapping_factor = render.renderer.getMappingFactor();
    if (ImGui::InputFloat("Mapping Factor", &mapping_factor)) {
      render.renderer.setMappingFactor(mapping_factor);
    }

    static float gamma = render.renderer.getGamma();
    if (ImGui::InputFloat("Gamma", &gamma)) {
      render.renderer.setGamma(gamma);
    }
  }
  ImGui::End();
}

void GUI::drawSkyUI(Render& render) const {
  ImGui::Begin("Sky");
  {
    static int sky_type = 0;
    ImGui::Combo("Sky Type", &sky_type, "Uniform\0Hosek\0");

    if (sky_type == 0) {
      static float color[3];
      ImGui::ColorEdit3("Color", color);
    } else if (sky_type == 1) {
      static float sunDirection[3];
      ImGui::InputFloat3("Sun Direction", sunDirection);

      static float turbidity = 0;
      ImGui::InputFloat("Turbidity", &turbidity);

      static float albedo[3];
      ImGui::ColorEdit3("Albedo", albedo);
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