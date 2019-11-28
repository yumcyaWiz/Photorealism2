cmake_minimum_required(VERSION 3.13..3.15)
cmake_policy(SET CMP0076 NEW)

#pthread
find_package(Threads REQUIRED)

#prl2
add_library(prl2 STATIC)
add_subdirectory(src)
add_subdirectory(extern)

# Include
target_include_directories(prl2 PUBLIC src)

# Link
target_link_libraries(prl2 PUBLIC Threads::Threads)
target_link_libraries(prl2 PUBLIC pool)
target_link_libraries(prl2 PUBLIC hosek)

#compile settings
target_compile_features(prl2 PUBLIC cxx_std_17)
set_target_properties(prl2 PROPERTIES CXX_EXTENSIONS OFF)

# compile definitions
add_definitions(
  -D_ENABLE_EXTENDED_ALIGNED_STORAGE
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
