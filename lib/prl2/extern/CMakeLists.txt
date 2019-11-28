# HosekWilkie
add_library(hosek
  HosekWilkie/ArHosekSkyModel.c
)
target_include_directories(hosek PUBLIC
  HosekWilkie
)
target_compile_features(hosek PRIVATE c_std_99)

# ThreadPool
add_library(pool INTERFACE)
target_include_directories(pool INTERFACE
  ThreadPool
)

# stb
add_library(stb INTERFACE)
target_compile_definitions(stb INTERFACE
  STB_IMAGE_IMPLEMENTATION
)
target_include_directories(stb INTERFACE
  stb
)

# tinyexr
add_library(tinyexr INTERFACE)
target_compile_definitions(tinyexr INTERFACE
  TINYEXR_IMPLEMENTATION
)
target_include_directories(tinyexr INTERFACE
  tinyexr
)