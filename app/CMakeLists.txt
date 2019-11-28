cmake_minimum_required(VERSION 3.12..3.15)

project(Photorealism2Viewer LANGUAGES C CXX)

#glad
add_library(glad STATIC extern/glad/src/glad.c)
target_include_directories(glad PUBLIC extern/glad/include)

#GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(extern/glfw)

#Dear ImGui
add_library(imgui STATIC 
  extern/imgui/imgui.cpp 
  extern/imgui/imgui_widgets.cpp 
  extern/imgui/imgui_draw.cpp 
  extern/imgui/imgui_demo.cpp
)
target_include_directories(imgui PUBLIC extern/imgui)

#Dear ImGui Impl
add_library(imgui_impl STATIC 
  extern/imgui/examples/imgui_impl_glfw.cpp 
  extern/imgui/examples/imgui_impl_opengl3.cpp
)
target_include_directories(imgui_impl PUBLIC extern/imgui/examples)
target_compile_definitions(imgui_impl PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)
target_link_libraries(imgui_impl PUBLIC imgui)
target_link_libraries(imgui_impl PUBLIC glfw)
target_link_libraries(imgui_impl PUBLIC glad)

#pthread
find_package(Threads REQUIRED)

#app
add_executable(app
  src/gui.cpp
  src/gui.h
  src/main.cpp
  src/render.cpp
  src/render.h
)
target_include_directories(app PRIVATE src/)

# compile settings
target_compile_features(app PRIVATE cxx_std_17)
set_target_properties(app PROPERTIES CXX_EXTENSIONS OFF)

# compile options
target_compile_options(app PRIVATE 
  $<$<CXX_COMPILER_ID:GNU>:
    -Wall -Wextra -pedantic-errors
    $<$<CONFIG:Debug>: -g3 -O0 -pg>
    $<$<CONFIG:Release>: -O3 -s -DNDEBUG -march=native -mtune=native>
  >
  $<$<CXX_COMPILER_ID:Clang>:
    -Wall -Wextra -pedantic-errors
    $<$<CONFIG:Debug>: -g3 -O0 -pg>
    $<$<CONFIG:Release>: -O3 -s -DNDEBUG -march=native -mtune=native>
  >
)

#app link
target_link_libraries(app PRIVATE glfw)
target_link_libraries(app PRIVATE glad)
target_link_libraries(app PRIVATE imgui)
target_link_libraries(app PRIVATE imgui_impl)
target_link_libraries(app PRIVATE Threads::Threads)
target_link_libraries(app PRIVATE prl2)