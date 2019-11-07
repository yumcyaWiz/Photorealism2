cmake_minimum_required(VERSION 3.12..3.15)

project(Photorealism2 LANGUAGES C CXX)

#pthread
find_package(Threads REQUIRED)

#prl2
add_library(prl2 STATIC 
  src/camera/film.h
  src/camera/film.cpp
  src/camera/camera.h
  src/camera/pinhole.h
  src/camera/pinhole.cpp
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
  src/intersector/intersector.h
  src/intersector/linear.h
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

#tests
file(GLOB TEST_SOURCES tests/*.cpp)
foreach(source_file ${TEST_SOURCES})
  get_filename_component(source_name ${source_file} NAME_WE)
  add_executable(${source_name} ${source_file})
  target_include_directories(${source_name} PRIVATE prl2)
  target_link_libraries(${source_name} PRIVATE prl2)
endforeach(source_file ${TEST_SOURCES})