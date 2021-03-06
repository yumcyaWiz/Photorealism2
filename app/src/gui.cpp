#include <string>

#include "imgui.h"

#include "gui.h"

#include "core/vec3.h"
#include "renderer/renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"

const std::string GUI::showpath_vertex_shader_source = R"(
  #version 330 core
  layout(location = 0) in vec3 vPos;

  uniform mat4 MVP;

  void main() {
    gl_Position = MVP * vec4(vPos, 1.0f);
  }
)";

const std::string GUI::showpath_fragment_shader_source = R"(
  #version 330 core
  layout(location = 0) out vec3 color;

  void main() {
    color = vec3(0.0f, 1.0f, 0.0f);
  }
)";

const std::string GUI::image_vertex_shader_source = R"(
  #version 330 core
  layout(location = 0) in vec3 vPos;
  layout(location = 1) in vec2 vTexCoord;

  out vec2 texCoord;

  void main() {
    gl_Position = vec4(vPos, 1.0f);
    texCoord = vTexCoord;
  }
)";

const std::string GUI::image_fragment_shader_source = R"(
  #version 330 core
  in vec2 texCoord;

  uniform sampler2D renderTexture;
  uniform sampler2D showPathTexture;

  layout(location = 0) out vec3 color;

  void main() {
    color = texture(renderTexture, texCoord).rgb + texture(showPathTexture, texCoord).rgb;
  }
)";

GUI::GUI() {
  // Render Textureの用意
  glGenTextures(1, &render_texture);

  // Show Path
  // Path Textureの用意
  glGenTextures(1, &showpath_texture);
  glBindTexture(GL_TEXTURE_2D, showpath_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE,
               0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glBindTexture(GL_TEXTURE_2D, 0);

  // FrameBufferの用意
  glGenFramebuffers(1, &showpath_framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, showpath_framebuffer);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, showpath_texture,
                       0);
  constexpr static GLenum draw_buffers[1] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, draw_buffers);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // Vertex Shader
  GLuint showpath_vert_shader =
      createShader(GL_VERTEX_SHADER, showpath_vertex_shader_source);

  // Fragment Shader
  GLuint showpath_frag_shader =
      createShader(GL_FRAGMENT_SHADER, showpath_fragment_shader_source);

  // Link Program
  showpath_program = createProgram(showpath_vert_shader, showpath_frag_shader);
  glDeleteShader(showpath_vert_shader);
  glDeleteShader(showpath_frag_shader);

  // VBO
  glGenBuffers(1, &showpath_vbo);

  // VAO
  glGenVertexArrays(1, &showpath_vao);
  glBindVertexArray(showpath_vao);
  // Vertex Positions
  glBindBuffer(GL_ARRAY_BUFFER, showpath_vbo);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT),
                        (GLvoid*)0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // Image
  // Image Textureの用意
  glGenTextures(1, &image_texture);
  glBindTexture(GL_TEXTURE_2D, image_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE,
               0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glBindTexture(GL_TEXTURE_2D, 0);

  // Image Frame Buffer
  glGenFramebuffers(1, &image_framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, image_framebuffer);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, image_texture, 0);
  static constexpr GLenum image_drawbuffers[] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, image_drawbuffers);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // Vertex Shader
  GLuint image_vertex_shader =
      createShader(GL_VERTEX_SHADER, image_vertex_shader_source);

  // Fragment Shader
  GLuint image_fragment_shader =
      createShader(GL_FRAGMENT_SHADER, image_fragment_shader_source);

  // Link Program
  image_program = createProgram(image_vertex_shader, image_fragment_shader);
  glDeleteShader(image_vertex_shader);
  glDeleteShader(image_fragment_shader);

  // Uniform Variables
  glUseProgram(image_program);
  glUniform1i(glGetUniformLocation(image_program, "renderTexture"), 0);
  glUniform1i(glGetUniformLocation(image_program, "showPathTexture"), 1);

  // Vertex VBO
  static constexpr float vertices[] = {
      1.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
      -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f,  0.0f, 0.0f, 1.0f};
  glGenBuffers(1, &image_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, image_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 20, vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // EBO
  static constexpr unsigned int indices[] = {0, 1, 3, 1, 2, 3};
  glGenBuffers(1, &image_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, image_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indices,
               GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // VAO
  glGenVertexArrays(1, &image_vao);
  glBindVertexArray(image_vao);

  // Vertex Attribute
  glBindBuffer(GL_ARRAY_BUFFER, image_vbo);
  // Position
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT),
                        (GLvoid*)0);
  // Texture Coordinate
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT),
                        (GLvoid*)(3 * sizeof(GL_FLOAT)));

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

GLuint GUI::createShader(GLenum shader_type,
                         const std::string& source_str) const {
  GLuint shader = glCreateShader(shader_type);

  const char* source = source_str.c_str();
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  int success;
  char infolog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infolog);
    std::cerr << infolog << std::endl;
  }

  return shader;
}

GLuint GUI::createProgram(GLuint vertex_shader, GLuint fragment_shader) const {
  GLuint program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);

  int success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    char infolog[512];
    glGetProgramInfoLog(program, 512, NULL, infolog);
    std::cerr << infolog << std::endl;
  }

  return program;
}

void GUI::drawRenderSettings(Render& render) {
  bool refresh_render = false;

  ImGui::Begin("Render Settings");
  {
    unsigned int sx, sy;
    render.renderer.getImageSize(sx, sy);
    static int size[2] = {static_cast<int>(sx), static_cast<int>(sy)};
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

    static bool _interactive = render.renderer.getRenderInteractive();
    if (ImGui::Checkbox("Interactive", &_interactive)) {
      render.renderer.setRenderInteractive(_interactive);
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
      if (render.renderer.getRenderInteractive()) {
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
    if (ImGui::Combo("Layer", &e,
                     "Render\0Denoise\0Albedo\0Normal\0UV\0Position\0Depth\0Sam"
                     "ple\0\0")) {
      if (e == 0) {
        render.renderer.setOutputLayer(Prl2::LayerType::Render);
      } else if (e == 1) {
        render.renderer.setOutputLayer(Prl2::LayerType::Denoise);
      } else if (e == 2) {
        render.renderer.setOutputLayer(Prl2::LayerType::Albedo);
      } else if (e == 3) {
        render.renderer.setOutputLayer(Prl2::LayerType::Normal);
      } else if (e == 4) {
        render.renderer.setOutputLayer(Prl2::LayerType::UV);
      } else if (e == 5) {
        render.renderer.setOutputLayer(Prl2::LayerType::Position);
      } else if (e == 6) {
        render.renderer.setOutputLayer(Prl2::LayerType::Depth);
      } else if (e == 7) {
        render.renderer.setOutputLayer(Prl2::LayerType::Sample);
      }
      update_texture = true;
    }

    // Render Texture生成
    unsigned int width, height;
    render.renderer.getImageSize(width, height);

    if (update_texture || render.isRendering()) {
      std::vector<float> image;
      render.renderer.getLayersRGB(image);
      makeTextureFromLayer(render_texture, width, height, image);
      update_texture = false;
    }

    // Image Textureの生成
    renderImageTexture();

    // テクスチャの表示
    ImTextureID id = (ImTextureID)(intptr_t)(image_texture);
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

      // パスの表示
      if (ImGui::IsMouseClicked(0)) {
        // クリックした画素の位置を計算
        const ImVec2 mouse_pos = ImGui::GetIO().MousePos;
        const int i = mouse_pos.x - image_pos.x;
        const int j = mouse_pos.y - image_pos.y;

        // パスの表示
        showPath(i, j, render);
      }
    }

    // sRGBの表示
    static float render_color[3];
    if (texture_hovered) {
      const int i = ImGui::GetIO().MousePos.x - image_pos.x;
      const int j = ImGui::GetIO().MousePos.y - image_pos.y;
      const Prl2::RGB rgb = render.renderer.getsRGB(i, j);
      render_color[0] = rgb.x();
      render_color[1] = rgb.y();
      render_color[2] = rgb.z();
    }
    ImGui::ColorEdit3("Render sRGB", render_color,
                      ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR);

    // Normalの表示
    static float hit_normal[3];
    if (texture_hovered) {
      const int i = ImGui::GetIO().MousePos.x - image_pos.x;
      const int j = ImGui::GetIO().MousePos.y - image_pos.y;
      const Prl2::Vec3 normal = render.renderer.getNormal(i, j);
      hit_normal[0] = normal.x();
      hit_normal[1] = normal.y();
      hit_normal[2] = normal.z();
    }
    ImGui::InputFloat3("Hit Normal", hit_normal);

    // Depthの表示
    static float hit_depth;
    if (texture_hovered) {
      const int i = ImGui::GetIO().MousePos.x - image_pos.x;
      const int j = ImGui::GetIO().MousePos.y - image_pos.y;
      hit_depth = render.renderer.getDepth(i, j);
    }
    ImGui::InputFloat("Depth", &hit_depth);

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

void GUI::drawIntegratorUI(Render& render) {
  ImGui::Begin("Integrator");
  {
    static int e = 0;
    Prl2::IntegratorType type = render.renderer.getIntegratorType();
    if (type == Prl2::IntegratorType::PT) {
      e = 0;
    } else if (type == Prl2::IntegratorType::NEE) {
      e = 1;
    }
    if (ImGui::Combo("Integrator Type", &e, "PT\0NEE\0\0")) {
      if (e == 0) {
        render.renderer.setIntegratorType(Prl2::IntegratorType::PT);
      } else if (e == 1) {
        render.renderer.setIntegratorType(Prl2::IntegratorType::NEE);
      }
    }
  }
  ImGui::End();
}

void GUI::renderImageTexture() const {
  glBindFramebuffer(GL_FRAMEBUFFER, image_framebuffer);
  glClear(GL_COLOR_BUFFER_BIT);
  glViewport(0, 0, 512, 512);

  glBindVertexArray(image_vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, image_ebo);

  glUseProgram(image_program);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, render_texture);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, showpath_texture);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
    if (ImGui::Combo("Sky Type", &sky_type, "Uniform\0Hosek\0IBL\0\0")) {
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
      const std::string prl2_filename = render.renderer.getIBLSkyFilename();
      strcpy(filename, prl2_filename.c_str());
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

void GUI::showPath(int i, int j, const Render& render) const {
  // パスの生成
  std::vector<Prl2::Ray> path;
  render.renderer.generatePath(i, j, path);

  // カメラパラメーターの取得
  Prl2::Vec3 camera_pos, camera_lookat;
  render.renderer.getCameraLookAt(camera_pos, camera_lookat);

  Prl2::Mat4 mat;
  render.renderer.getCameraMatrix(mat);

  const float fov = render.renderer.getPinholeCameraFOV();

  // MVP行列をセット
  // View Matrix
  /*
  const glm::mat4x4 view_matrix = glm::lookAt(
      glm::vec3(camera_pos.x(), camera_pos.y(), camera_pos.z()),
      glm::vec3(camera_lookat.x(), camera_lookat.y(), camera_lookat.z()),
      glm::vec3(0.0f, 1.0f, 0.0f));
      */
  const glm::mat4x4 view_matrix = glm::mat4x4(
      glm::vec4(mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0]),
      glm::vec4(mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1]),
      glm::vec4(mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2]),
      glm::vec4(mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]));

  // Projection Matrix
  const glm::mat4x4 projection_matrix =
      glm::infinitePerspective(glm::radians(fov), 1.0f, 0.000001f);

  // MVP Matrix
  const glm::mat4x4 mvp_matrix = projection_matrix * view_matrix;

  // パスの頂点データをシェーダーに渡す
  std::vector<float> vertices;
  for (int i = 0; i < path.size(); ++i) {
    const Prl2::Ray& ray = path[i];
    vertices.push_back(ray.origin.x());
    vertices.push_back(ray.origin.y());
    vertices.push_back(ray.origin.z());
  }

  // Vertex VBO
  glBindBuffer(GL_ARRAY_BUFFER, showpath_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(),
               vertices.data(), GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Draw
  glBindFramebuffer(GL_FRAMEBUFFER, showpath_framebuffer);
  glUseProgram(showpath_program);

  glViewport(0, 0, 512, 512);
  glClear(GL_COLOR_BUFFER_BIT);

  glUniformMatrix4fv(glGetUniformLocation(showpath_program, "MVP"), 1, GL_FALSE,
                     glm::value_ptr(mvp_matrix));

  glBindVertexArray(showpath_vao);
  glDrawArrays(GL_LINES, 0, vertices.size() / 3);
  glBindVertexArray(0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}