cmake_minimum_required(VERSION 3.13..3.15)
cmake_policy(SET CMP0076 NEW)

#pthread
find_package(Threads REQUIRED)

#prl2
add_library(prl2 STATIC)
add_subdirectory(src)
target_include_directories(prl2 PUBLIC include/)
target_include_directories(prl2 PUBLIC src/)
target_link_libraries(prl2 PUBLIC Threads::Threads)

#compile options
target_compile_features(prl2 PUBLIC cxx_std_17)
set_target_properties(prl2 PROPERTIES CXX_EXTENSIONS OFF)
target_compile_options(prl2 PRIVATE 
  -Wall -Wextra -pedantic-errors
  $<$<CONFIG:Debug>: -g3 -O0 -pg>
  $<$<CONFIG:Release>: -O3 -s -DNDEBUG -march=native -mtune=native>
)

#tests
file(GLOB TEST_SOURCES tests/*.cpp)
foreach(source_file ${TEST_SOURCES})
  get_filename_component(source_name ${source_file} NAME_WE)
  add_executable(${source_name} ${source_file})
  target_include_directories(${source_name} PRIVATE prl2)
  target_link_libraries(${source_name} PRIVATE prl2)
endforeach(source_file ${TEST_SOURCES})