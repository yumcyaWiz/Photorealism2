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

    static bool _realtime = render.renderer.getRenderRealtime();
    if (ImGui::Checkbox("Realtime", &_realtime)) {
      render.renderer.setRenderRealtime(_realtime);
    }
    ImGui::SameLine();

    static bool _auto_render = auto_render;
    if (ImGui::Checkbox("Auto Refresh", &_auto_render)) {
      auto_render = _auto_render;
    }

    // レンダリング中
    if (render.isRendering()) {
      // キャンセルボタン
      if (ImGui::Button("Cancel")) {
        render.cancelRender();
      }
      ImGui::SameLine();

      // Progress Bar
      const float progress = render.renderer.getRenderProgress();
      if (render.renderer.getRenderRealtime()) {
        ImGui::ProgressBar(progress, ImVec2(-1, 0),
                           (std::to_string(static_cast<int>(
                                render.renderer.getSamples() * progress)) +
                            " spp")
                               .c_str());
      } else {
        ImGui::ProgressBar(progress);
      }
    }
    // レンダリングしていない状態
    else {
      // Renderボタン
      if (ImGui::Button("Render")) {
        render.requestRender();
      }
      ImGui::SameLine();

      // レンダリングに要した時間
      ImGui::Text((std::to_string(render.renderer.getRenderingTime()) + " [ms]")
                      .c_str());
    }

    // Denoise Button
    if (ImGui::Button("Denoise")) {
      render.renderer.denoise();
    }

    ImGui::Separator();

    static char filename[32];
    ImGui::InputText("Image Filename", filename, 32);

    static int image_type = 0;
    ImGui::Combo("Image Type", &image_type, "PPM\0PNG\0EXR\0HDR\0PFM\0\0");

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
      } else if (image_type == 4) {
        render.renderer.setImageType(Prl2::ImageType::PFM);
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

void GUI::drawRenderLayer(Render& render) {
  ImGui::Begin("Render");
  {
    // Layer選択ボタン
    static int e = 0;
    if (ImGui::Combo(
            "Layer", &e,
            "Render\0Denoise\0Albedo\0Normal\0Position\0Depth\0Sample\0\0")) {
      if (e == 0) {
        render.renderer.setOutputLayer(Prl2::LayerType::Render);
      } else if (e == 1) {
        render.renderer.setOutputLayer(Prl2::LayerType::Denoise);
      } else if (e == 2) {
        render.renderer.setOutputLayer(Prl2::LayerType::Albedo);
      } else if (e == 3) {
        render.renderer.setOutputLayer(Prl2::LayerType::Normal);
      } else if (e == 4) {
        render.renderer.setOutputLayer(Prl2::LayerType::Position);
      } else if (e == 5) {
        render.renderer.setOutputLayer(Prl2::LayerType::Depth);
      } else if (e == 6) {
        render.renderer.setOutputLayer(Prl2::LayerType::Sample);
      }
      update_texture = true;
    }

    // テクスチャの生成
    int width, height;
    render.renderer.getImageSize(width, height);

    if (update_texture || render.isRendering()) {
      std::vector<float> image;
      render.renderer.getLayersRGB(image);
      makeTextureFromLayer(render_texture_id, width, height, image);
      update_texture = false;
    }

    // テクスチャの表示
    ImTextureID id = (ImTextureID)(intptr_t)(render_texture_id);
    const ImVec2 image_pos = ImGui::GetCursorScreenPos();
    ImGui::Image(id, ImVec2(512, 512));
    bool texture_hovered = ImGui::IsItemHovered();

    if (texture_hovered) {
      // カメラ移動XY(Shift + Mouse)
      if (ImGui::IsMouseDragging() && ImGui::IsKeyDown(340)) {
        const ImVec2 delta = ImGui::GetIO().MouseDelta;

        Prl2::Vec3 pos_diff(1e-2 * delta.x, 1e-2 * delta.y, 0);
        render.renderer.moveCamera(pos_diff);

        render.requestRender();
      }
      // カメラ回転
      else if (ImGui::IsMouseDragging()) {
        const ImVec2 delta = ImGui::GetIO().MouseDelta;

        Prl2::Vec3 rotate(1e-2 * delta.y, 1e-2 * delta.x, 0);
        render.renderer.rotateCamera(rotate);

        render.requestRender();
      }

      // カメラ移動Z(Mouse Scroll)
      {
        const float delta = ImGui::GetIO().MouseWheel;
        if (delta != 0) {
          Prl2::Vec3 pos_diff(0, 0, -1e-1 * delta);
          render.renderer.moveCamera(pos_diff);

          render.requestRender();
        }
      }
    }

    // SPDの表示
    static const float* phi = render.renderer.getSPD(0, 0).phi.data();
    if (texture_hovered) {
      const int i = ImGui::GetIO().MousePos.x - image_pos.x;
      const int j = ImGui::GetIO().MousePos.y - image_pos.y;
      phi = render.renderer.getSPD(i, j).phi.data();
    }
    ImGui::PlotHistogram(
        "SPD", phi, Prl2::SPD::LAMBDA_SAMPLES, 0, nullptr, 0,
        *std::max_element(phi, phi + Prl2::SPD::LAMBDA_SAMPLES), ImVec2(0, 50));
  }
  ImGui::End();
}

void GUI::drawCameraSettings(Render& render) const {
  bool refresh_render = false;

  ImGui::Begin("Camera");
  {
    // LookAt
    Prl2::Vec3 pos, lookat;
    render.renderer.getCameraLookAt(pos, lookat);

    bool lookat_modified = false;

    static float camera_position[3];
    camera_position[0] = pos.x();
    camera_position[1] = pos.y();
    camera_position[2] = pos.z();
    lookat_modified |= ImGui::InputFloat3("Camera Position", camera_position);

    static float lookat_position[3];
    lookat_position[0] = lookat.x();
    lookat_position[1] = lookat.y();
    lookat_position[2] = lookat.z();
    lookat_modified |= ImGui::InputFloat3("Lookat", lookat_position);

    if (lookat_modified) {
      render.renderer.setCameraLookAt(
          Prl2::Vec3(camera_position[0], camera_position[1],
                     camera_position[2]),
          Prl2::Vec3(lookat_position[0], lookat_position[1],
                     lookat_position[2]));
      refresh_render = true;
    }

    // Camera Type
    static int camera_type = 0;
    const Prl2::CameraType prl2_camera_type = render.renderer.getCameraType();
    switch (prl2_camera_type) {
      case Prl2::CameraType::Pinhole:
        camera_type = 0;
        break;
      case Prl2::CameraType::Environment:
        camera_type = 1;
        break;
      case Prl2::CameraType::ThinLens:
        camera_type = 2;
        break;
    }
    if (ImGui::Combo("Camera Type", &camera_type,
                     "Pinhole\0Environment\0ThinLens\0\0")) {
      if (camera_type == 0) {
        render.renderer.setCameraType(Prl2::CameraType::Pinhole);
      } else if (camera_type == 1) {
        render.renderer.setCameraType(Prl2::CameraType::Environment);
      } else if (camera_type == 2) {
        render.renderer.setCameraType(Prl2::CameraType::ThinLens);
      }
    }

    // Pinhole Camera
    if (camera_type == 0) {
      static float fov = render.renderer.getPinholeCameraFOV();
      if (ImGui::InputFloat("FOV", &fov)) {
        render.renderer.setPinholeCameraFOV(fov);
      }
    }
    // Environment Camera
    else if (camera_type == 1) {
    }
    // Thin Lens Camera
    else if (camera_type == 2) {
      static float fov = render.renderer.getThinLensCameraFOV();
      if (ImGui::InputFloat("FOV", &fov)) {
        render.renderer.setThinLensCameraFOV(fov);
      }

      static float radius = render.renderer.getThinLensCameraLensRadius();
      if (ImGui::InputFloat("Lens Radius", &radius)) {
        render.renderer.setThinLensCameraLensRadius(radius);
      }

      static float distance = render.renderer.getThinLensCameraFocusDistance();
      if (ImGui::InputFloat("Focus Distance", &distance)) {
        render.renderer.setThinLensCameraFocusDistance(distance);
      }
    }

    // Commit Camera
    if (ImGui::Button("Commit Camera")) {
      render.renderer.commitCamera();
    }

    ImGui::Separator();

    // Film Length
    float lx, ly;
    render.renderer.getFilmLength(lx, ly);
    static float film_length[2] = {lx, ly};
    if (ImGui::InputFloat2("Film Length [m]", film_length)) {
      render.renderer.setFilmLength(film_length[0], film_length[1]);
      refresh_render = true;
    }
  }
  ImGui::End();

  if (refresh_render && auto_render) {
    render.requestRender();
  }
}

void GUI::drawToneMappingUI(Render& render) {
  ImGui::Begin("Tone Mapping");
  {
    static float exposure = render.renderer.getExposure();
    if (ImGui::InputFloat("Exposure", &exposure)) {
      render.renderer.setExposure(exposure);
      update_texture = true;
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
      update_texture = true;
    }

    static float mapping_factor = render.renderer.getMappingFactor();
    if (ImGui::InputFloat("Mapping Factor", &mapping_factor)) {
      render.renderer.setMappingFactor(mapping_factor);
      update_texture = true;
    }

    static float gamma = render.renderer.getGamma();
    if (ImGui::InputFloat("Gamma", &gamma)) {
      render.renderer.setGamma(gamma);
      update_texture = true;
    }
  }
  ImGui::End();
}

void GUI::drawSkyUI(Render& render) const {
  ImGui::Begin("Sky");
  {
    // Sky Type
    static int sky_type = 0;
    switch (render.renderer.getSkyType()) {
      case Prl2::SkyType::Uniform:
        sky_type = 0;
        break;
      case Prl2::SkyType::Hosek:
        sky_type = 1;
        break;
      case Prl2::SkyType::IBL:
        sky_type = 2;
        break;
    }
    if (ImGui::Combo("Sky Type", &sky_type, "Uniform\0Hosek\0\0")) {
      if (sky_type == 0) {
        render.renderer.setSkyType(Prl2::SkyType::Uniform);
      } else if (sky_type == 1) {
        render.renderer.setSkyType(Prl2::SkyType::Hosek);
      } else if (sky_type == 2) {
        render.renderer.setSkyType(Prl2::SkyType::IBL);
      }
    }

    // Uniform Sky
    if (sky_type == 0) {
      static float color[3];
      const Prl2::Vec3 prl2_uniform_sky_color =
          render.renderer.getUniformSkyColor();
      color[0] = prl2_uniform_sky_color.x();
      color[1] = prl2_uniform_sky_color.y();
      color[2] = prl2_uniform_sky_color.z();
      if (ImGui::ColorEdit3(
              "Color", color,
              ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR)) {
        render.renderer.setUniformSkyColor(
            Prl2::Vec3(color[0], color[1], color[2]));
      }
    }
    // Hosek Sky
    else if (sky_type == 1) {
      // Sun Direction
      static float sunDirection[3];
      const Prl2::Vec3 prl2_sun_direction =
          render.renderer.getHosekSkySunDirection();
      sunDirection[0] = prl2_sun_direction.x();
      sunDirection[1] = prl2_sun_direction.y();
      sunDirection[2] = prl2_sun_direction.z();
      if (ImGui::InputFloat3("Sun Direction", sunDirection)) {
        render.renderer.setHosekSkySunDirection(
            Prl2::Vec3(sunDirection[0], sunDirection[1], sunDirection[2]));
      }

      // Turbidity
      static float turbidity;
      turbidity = render.renderer.getHosekSkyTurbidity();
      if (ImGui::InputFloat("Turbidity", &turbidity)) {
        render.renderer.setHosekSkyTurbidity(turbidity);
      }

      // Albedo
      static float albedo[3];
      const Prl2::Vec3 prl2_albedo = render.renderer.getHosekSkyAlbedo();
      albedo[0] = prl2_albedo.x();
      albedo[1] = prl2_albedo.y();
      albedo[2] = prl2_albedo.z();
      if (ImGui::ColorEdit3(
              "Albedo", albedo,
              ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR)) {
        render.renderer.setHosekSkyAlbedo(
            Prl2::Vec3(albedo[0], albedo[1], albedo[2]));
      }
    }
    // IBL Sky
    else if (sky_type == 2) {
      // IBL filename
      static char filename[32];
      if (ImGui::InputText("Filename", filename, 32)) {
        render.renderer.setIBLSkyFilename(std::string(filename));
      }
    }

    // Commit Sky
    if (ImGui::Button("Commit Sky")) {
      render.renderer.commitSky();
    }
  }
  ImGui::End();
}

void GUI::makeTextureFromLayer(GLuint texture_id, int width, int height,
                               const std::vector<float>& rgb) const {
  if (rgb.size() != static_cast<unsigned int>(3 * width * height)) return;

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