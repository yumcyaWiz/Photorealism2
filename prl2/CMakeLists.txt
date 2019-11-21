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
target_compile_definitions(prl2 PRIVATE _USE_MATH_DEFINES)
target_compile_options(prl2 PRIVATE 
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