cmake_minimum_required(VERSION 3.13..3.15)
cmake_policy(SET CMP0076 NEW)

#pthread
find_package(Threads REQUIRED)

#prl2
add_library(prl2 SHARED)
add_subdirectory(camera)
add_subdirectory(core)
add_subdirectory(integrator)
add_subdirectory(intersector)
add_subdirectory(shape)

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