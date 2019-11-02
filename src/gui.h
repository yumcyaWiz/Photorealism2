#ifndef GUI_H
#define GUI_H

#include <iostream>
#include <string>

#include "glad/glad.h"

#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

static void glfw_error_callback(int error, const char* desc) {
  fprintf(stderr, "glfw error %d: %s\n", error, desc);
}

class GUI {
 public:
  GLFWwindow* render_window;  //レンダリング表示用のウインドウ
  GLFWwindow* gui_window;     // GUI用のウィンドウ

  // GUIを初期化
  GUI() {
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
    gui_window = glfwCreateWindow(512, 512, "GUI", NULL, NULL);
    if (gui_window == nullptr) {
      std::cerr << "failed to create window" << std::endl;
      glfwTerminate();
      std::exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(gui_window);
    glfwSwapInterval(1);

    // GLADの初期化
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cerr << "failed to initialize glad" << std::endl;
      glfwTerminate();
      std::exit(EXIT_FAILURE);
    }
  };

  //レンダリングループ
  //この関数をstd::threadに渡す
  static void render() {
    while (1) {
    }
  }
};

#endif