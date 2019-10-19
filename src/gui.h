#ifndef GUI_H
#define GUI_H

#include <iostream>

#include <GLFW/glfw3.h>
#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"


static void glfw_error_callback(int error, const char* desc) {
  fprintf(stderr, "glfw error %d: %s\n", error, desc);
}


class GUI {
  public:

  GLFWwindow* render_window; //レンダリング表示用のウインドウ
  GLFWwindow* gui_window; //GUI用のウィンドウ

  constexpr static char* glsl_version = "#version 130";

  //GUIを初期化
  GUI() {
    //GLFWを初期化
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
      std::cerr << "failed to initialize glfw context" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    //Renderウィンドウの生成
    render_window = glfwCreateWindow(512, 512, "Render", NULL, NULL);
    if (render_window == nullptr) {
      std::cerr << "failed to create window" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(render_window);
    glfwSwapInterval(1);

    //GUIウインドウの作成
    gui_window = glfwCreateWindow(512, 512, "GUI", NULL, NULL);
    if (gui_window == nullptr) {
      std::cerr << "failed to create window" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(gui_window);
    glfwSwapInterval(1);
  };

  //レンダリングループ
  //この関数をstd::threadに渡す
  static void render() {
    while(1) {

    }
  }
};


#endif