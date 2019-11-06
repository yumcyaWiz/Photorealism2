cmake_minimum_required(VERSION 3.12..3.15)

project(Photorealism2 LANGUAGES C CXX)

#pthread
find_package(Threads REQUIRED)

#prl2
add_library(prl2 STATIC 
  src/core/film.cpp
  src/core/film.h
  src/core/isect.h
  src/core/mat4.h
  src/core/primitive.h
  src/core/ray.h
  src/core/spectrum.cpp
  src/core/spectrum.h
  src/core/transform.h
  src/core/type.h
  src/core/vec2.h
  src/core/vec3.h
  src/core/vec4.h
  src/shape/shape.h
  src/shape/sphere.h
  src/shape/sphere.cpp
)
target_include_directories(prl2 PUBLIC include/)
target_include_directories(prl2 PUBLIC src/)

#compile options
target_compile_features(prl2 PUBLIC cxx_std_17)
set_target_properties(prl2 PROPERTIES CXX_EXTENSIONS OFF)
target_compile_options(prl2 PRIVATE 
  -Wall -Wextra -pedantic-errors
  $<$<CONFIG:Debug>: -g3 -O0 -pg>
  $<$<CONFIG:Release>: -O3 -s -DNDEBUG -march=native -mtune=native>
)

#link
target_link_libraries(prl2 PUBLIC Threads::Threads)

#main
add_executable(main main.cpp)
target_include_directories(main PRIVATE prl2)
target_link_libraries(main PRIVATE prl2)
