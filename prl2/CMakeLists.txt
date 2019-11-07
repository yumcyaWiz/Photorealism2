cmake_minimum_required(VERSION 3.13..3.15)

add_subdirectory(src)
target_include_directories(prl2 PUBLIC include/)
target_include_directories(prl2 PUBLIC src/)

#tests
file(GLOB TEST_SOURCES tests/*.cpp)
foreach(source_file ${TEST_SOURCES})
  get_filename_component(source_name ${source_file} NAME_WE)
  add_executable(${source_name} ${source_file})
  target_include_directories(${source_name} PRIVATE prl2)
  target_link_libraries(${source_name} PRIVATE prl2)
endforeach(source_file ${TEST_SOURCES})