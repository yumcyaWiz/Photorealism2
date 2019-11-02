cmake_minimum_required(VERSION 3.15)

project(photorealism2 LANGUAGES C CXX)

#main
add_executable(main src/main.cpp)

#compile options
target_compile_features(main PUBLIC cxx_std_17)
set_target_properties(main PROPERTIES CXX_EXTENSION OFF)
target_compile_options(main PUBLIC 
  -Wall -Wextra -pedantic-errors
  $<$<CONFIG:Debug>: -g3 -O0 -pg>
  $<$<CONFIG:Release>: -O3 -s -DNDEBUG -march=native -mtune=native>
)

#OpenGL
find_package(OpenGL REQUIRED)

#GLFW
find_package(glfw3 3.3 REQUIRED)

#Dear ImGui
add_library(imgui STATIC ext/imgui/imgui.cpp ext/imgui/imgui_widgets.cpp ext/imgui/imgui_draw.cpp ext/imgui/imgui_demo.cpp)
target_include_directories(imgui PUBLIC ext/imgui)

#Dear ImGui Impl
add_library(imgui_impl STATIC ext/imgui/examples/imgui_impl_glfw.cpp ext/imgui/examples/imgui_impl_opengl3.cpp)
target_include_directories(imgui_impl PUBLIC ext/imgui)
target_include_directories(imgui_impl PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(imgui_impl imgui)

#pthread
find_package(Threads REQUIRED)

#main include 
target_include_directories(main PUBLIC ${OPENGL_INCLUDE_DIR})
target_include_directories(main PRIVATE imgui)
target_include_directories(main PRIVATE imgui_impl)

#main link
target_link_libraries(main glfw)
target_link_libraries(main ${OPENGL_LIBRARY})
target_link_libraries(main imgui)
target_link_libraries(main imgui_impl)
target_link_libraries(main Threads::Threads)