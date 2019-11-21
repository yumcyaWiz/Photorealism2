cmake_minimum_required(VERSION 3.13..3.15)
cmake_policy(SET CMP0076 NEW)

#pthread
find_package(Threads REQUIRED)

#prl2
add_library(prl2 STATIC)
add_subdirectory(src)

# Include
target_include_directories(prl2 PUBLIC src/)
target_include_directories(prl2 PUBLIC ext/)

# Link
target_link_libraries(prl2 PUBLIC Threads::Threads)

#compile settings
target_compile_features(prl2 PUBLIC cxx_std_17)
set_target_properties(prl2 PROPERTIES CXX_EXTENSIONS OFF)

# compile definitions
target_compile_definitions(prl2 PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:
    _ENABLE_EXTENDED_ALIGNED_STORAGE
  >
)

#compile options
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