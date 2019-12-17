cmake_minimum_required(VERSION 3.12..3.15)

project(Photorealism2Viewer LANGUAGES C CXX)

# pthread
find_package(Threads REQUIRED)

# extern
add_subdirectory(extern)

# app
add_executable(app
  src/gui.cpp
  src/main.cpp
  src/render.cpp
)
add_dependencies(app prl2)
target_include_directories(app PRIVATE src/)

# compile settings
target_compile_features(app PRIVATE cxx_std_17)
set_target_properties(app PROPERTIES CXX_EXTENSIONS OFF)

# compile options
target_compile_options(app PRIVATE 
  $<$<CXX_COMPILER_ID:GNU>:
    -Wall -Wextra -pedantic-errors
    $<$<CONFIG:Debug>: -g3 -O0 -pg>
    $<$<CONFIG:Release>: -O2 -ftree-vectorize -s -DNDEBUG -march=native -mtune=native>
  >
  $<$<CXX_COMPILER_ID:Clang>:
    -Wall -Wextra -pedantic-errors
    $<$<CONFIG:Debug>: -g3 -O0 -pg>
    $<$<CONFIG:Release>: -O2 -ftree-vectorize -s -DNDEBUG -march=native -mtune=native>
  >
)

#app link
target_link_libraries(app PRIVATE glfw)
target_link_libraries(app PRIVATE glad)
target_link_libraries(app PRIVATE imgui)
target_link_libraries(app PRIVATE imgui_impl)
target_link_libraries(app PRIVATE Threads::Threads)
target_link_libraries(app PRIVATE prl2)
target_link_libraries(app PRIVATE glm)