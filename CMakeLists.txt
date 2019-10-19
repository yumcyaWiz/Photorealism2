cmake_minimum_required(VERSION 3.12)

project(photorealism2 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 -pg")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -s -DNDEBUG -march=native -mtune=native")

add_executable(main src/main.cpp)

#OpenGL
find_package(OpenGL REQUIRED)
target_include_directories(main PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(main ${OPENGL_LIBRARY})

#GLFW
find_package(glfw3 3.3 REQUIRED)
target_link_libraries(main glfw)

#Dear ImGui
add_library(imgui STATIC ext/imgui/imgui.cpp ext/imgui/imgui_widgets.cpp ext/imgui/imgui_draw.cpp ext/imgui/imgui_demo.cpp ext/imgui/examples/imgui_impl_glfw.cpp ext/imgui/examples/imgui_impl_opengl3.cpp)
target_include_directories(imgui PUBLIC ext/imgui)
target_include_directories(imgui PUBLIC ${OPENGL_INCLUDE_DIR})

target_include_directories(main PUBLIC imgui)
target_link_libraries(main imgui)

#pthread
find_package(Threads REQUIRED)
target_link_libraries(main Threads::Threads)
