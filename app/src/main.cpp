#include <iostream>
#include <thread>

#include "glad/glad.h"

#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "gui.h"
#include "render.h"

static void glfw_error_callback(int error, const char* desc) {
  fprintf(stderr, "glfw error %d: %s\n", error, desc);
}

int main() {
  // GLFWを初期化
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) {
    std::cerr << "failed to initialize glfw context" << std::endl;
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  const char* glsl_version = "#version 150";

  // GUIウインドウの作成
  GLFWwindow* window = glfwCreateWindow(512, 512, "GUI", NULL, NULL);
  if (window == nullptr) {
    std::cerr << "failed to create window" << std::endl;
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  // GLADの初期化
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "failed to initialize glad" << std::endl;
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  }

  // ImGuiの初期化
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.ConfigWindowsMoveFromTitleBarOnly = true;

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // Renderの初期化
  Prl2::RenderConfig config;
  Render render(config);

  // レンダリングスレッドの作成
  std::thread rendering_thread = render.render();

  // GUIのレンダリングループ
  GUI gui;
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    gui.drawRenderSettings(render);
    gui.drawRenderLayer(render);
    gui.drawFilmSettings(render);
    gui.drawCameraSettings(render);
    gui.drawToneMappingUI(render);
    gui.drawSkyUI(render);

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  // ImGuiの終了処理
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  // GLFWの終了処理
  glfwDestroyWindow(window);
  glfwTerminate();

  // スレッドの終了処理
  rendering_thread.join();

  return 0;
}
