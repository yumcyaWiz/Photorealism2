cmake_minimum_required(VERSION 3.12..3.15)

project(Photorealism2Viewer LANGUAGES C CXX)

#glad
add_library(glad STATIC ext/glad/src/glad.c)
target_include_directories(glad PUBLIC ext/glad/include)

#GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(ext/glfw)

#Dear ImGui
add_library(imgui STATIC 
  ext/imgui/imgui.cpp 
  ext/imgui/imgui_widgets.cpp 
  ext/imgui/imgui_draw.cpp 
  ext/imgui/imgui_demo.cpp
)
target_include_directories(imgui PUBLIC ext/imgui)

#Dear ImGui Impl
add_library(imgui_impl STATIC 
  ext/imgui/examples/imgui_impl_glfw.cpp 
  ext/imgui/examples/imgui_impl_opengl3.cpp
)
target_include_directories(imgui_impl PUBLIC ext/imgui/examples)
target_compile_definitions(imgui_impl PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)
target_link_libraries(imgui_impl PUBLIC imgui)
target_link_libraries(imgui_impl PUBLIC glfw)
target_link_libraries(imgui_impl PUBLIC glad)

#pthread
find_package(Threads REQUIRED)

#Photorealism2
add_subdirectory(prl2)

#app
add_executable(app
  app/src/gui.cpp
  app/src/gui.h
  app/src/main.cpp
  app/src/render.cpp
  app/src/render.h
)
target_include_directories(app PRIVATE app/src/)

# compile settings
target_compile_features(app PRIVATE cxx_std_17)
set_target_properties(app PROPERTIES CXX_EXTENSIONS OFF)

# compile definitions
target_compile_definitions(app PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:
    _ENABLE_EXTENDED_ALIGNED_STORAGE
  >
)

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
target_link_libraries(app PRIVATE dl)
target_link_libraries(app PRIVATE imgui)
target_link_libraries(app PRIVATE imgui_impl)
target_link_libraries(app PRIVATE Threads::Threads)
target_link_libraries(app PRIVATE prl2)

#tests
set(TEST_SOURCES
  tests/draw_diffuse.cpp
)
foreach(source_file ${TEST_SOURCES})
  get_filename_component(source_name ${source_file} NAME_WE)
  add_executable(${source_name} ${source_file})
  target_include_directories(${source_name} PRIVATE prl2)
  target_link_libraries(${source_name} PRIVATE prl2)
endforeach(source_file ${TEST_SOURCES})