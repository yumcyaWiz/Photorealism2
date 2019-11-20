#include <iostream>
#include <thread>

#include "glad/glad.h"

#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "camera/pinhole.h"
#include "integrator/pt.h"
#include "intersector/linear.h"
#include "material/diffuse.h"
#include "renderer/renderer.h"
#include "shape/sphere.h"

using namespace Prl2;

Scene initScene(const RenderConfig& config) {
  const auto film = std::make_shared<Film>(config.width, config.height);
  const auto camera_trans =
      std::make_shared<Transform>(translate(Vec3(0, 0, 0)));
  const auto camera =
      std::make_shared<PinholeCamera>(film, camera_trans, 90.0f);

  const auto shape = std::make_shared<Sphere>(1.0f);
  const auto mat = std::make_shared<Diffuse>(SPD(0.9));
  const auto prim_trans =
      std::make_shared<Transform>(translate(Vec3(0, 0, -3)));
  const auto prim = std::make_shared<Primitive>(shape, mat, prim_trans);

  const auto shape2 = std::make_shared<Sphere>(10000.0f);
  const auto prim2_trans =
      std::make_shared<Transform>(translate(Vec3(0, -10001, 0)));
  const auto prim2 = std::make_shared<Primitive>(shape2, mat, prim2_trans);

  std::shared_ptr<LinearIntersector> intersector =
      std::make_shared<LinearIntersector>();
  intersector->addPrimitive(prim);
  intersector->addPrimitive(prim2);

  Scene scene(camera, intersector);
  return scene;
}

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

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // Rendererの初期化
  RenderConfig config;
  RenderLayer layer(config);
  Renderer renderer;

  // シーンの初期化
  Scene scene = initScene(config);

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

  // ImGuiの終了処理
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  // GLFWの終了処理
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
