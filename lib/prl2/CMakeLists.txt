cmake_minimum_required(VERSION 3.13..3.15)
cmake_policy(SET CMP0076 NEW)

#pthread
find_package(Threads REQUIRED)

#prl2
add_library(prl2)
add_subdirectory(src)
add_subdirectory(extern)

# Include
target_include_directories(prl2 PUBLIC src)

# Link
target_link_libraries(prl2 PUBLIC Threads::Threads)
target_link_libraries(prl2 PUBLIC pool)
target_link_libraries(prl2 PUBLIC hosek)
target_link_libraries(prl2 PUBLIC stb)
target_link_libraries(prl2 PUBLIC tinyexr)
target_link_libraries(prl2 PUBLIC OpenImageDenoise)
target_link_libraries(prl2 PUBLIC tinyobjloader)

#compile settings
target_compile_features(prl2 PUBLIC cxx_std_17)
set_target_properties(prl2 PROPERTIES CXX_EXTENSIONS OFF)

#compile options
target_compile_options(prl2 PRIVATE 
  $<$<CXX_COMPILER_ID:GNU>:
    $<$<CONFIG:Debug>: -g3 -O0 -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused>
    $<$<CONFIG:Release>: -O2 -ftree-vectorize -s -DNDEBUG -march=native -mtune=native>
  >
  $<$<CXX_COMPILER_ID:Clang>:
    $<$<CONFIG:Debug>: -g3 -O0 -pg -Werror -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic>
    $<$<CONFIG:Release>: -O2 -ftree-vectorize -s -DNDEBUG -march=native -mtune=native>
  >
)
