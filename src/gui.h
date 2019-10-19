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

  GUI() {
    //GLFWを初期化
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
      std::cerr << "failed to initialize glfw context" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    //ウィンドウの生成
    render_window = glfwCreateWindow(512, 512, "Render", NULL, NULL);
    if (render_window == NULL) {
      std::cerr << "failed to create window" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(render_window);
    glfwSwapInterval(1);
  };

  static void render() {
    while(1) {

    }
  }
};


#endif