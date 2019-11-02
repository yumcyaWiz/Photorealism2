#include <iostream>
#include <thread>

#include "glad/glad.h"

#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "film.h"
#include "mat4.h"
#include "ray.h"
#include "transform.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"

static void glfw_error_callback(int error, const char* desc) {
  fprintf(stderr, "glfw error %d: %s\n", error, desc);
}

int main() {
  Film film(512, 512);
  film.writePPM("output.ppm");

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

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // GUIのレンダリングループ
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  return 0;
}
