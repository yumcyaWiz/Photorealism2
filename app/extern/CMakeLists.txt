cmake_minimum_required(VERSION 3.12..3.15)

#glad
add_library(glad STATIC glad/src/glad.c)
target_include_directories(glad PUBLIC glad/include)

#GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)

#Dear ImGui
add_library(imgui STATIC 
  imgui/imgui.cpp 
  imgui/imgui_widgets.cpp 
  imgui/imgui_draw.cpp 
  imgui/imgui_demo.cpp
)
target_include_directories(imgui PUBLIC imgui)

#Dear ImGui Impl
add_library(imgui_impl STATIC 
  imgui/examples/imgui_impl_glfw.cpp 
  imgui/examples/imgui_impl_opengl3.cpp
)
target_include_directories(imgui_impl PUBLIC imgui/examples)
target_compile_definitions(imgui_impl PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)
target_link_libraries(imgui_impl PUBLIC imgui)
target_link_libraries(imgui_impl PUBLIC glfw)
target_link_libraries(imgui_impl PUBLIC glad)