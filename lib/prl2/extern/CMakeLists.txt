# HosekWilkie
add_library(hosek HosekWilkie/ArHosekSkyModel.c)
target_include_directories(hosek SYSTEM PUBLIC HosekWilkie)
target_compile_features(hosek PRIVATE c_std_99)

# ThreadPool
add_library(pool INTERFACE)
target_include_directories(pool SYSTEM INTERFACE ThreadPool)

# stb
add_library(stb INTERFACE)
target_compile_definitions(stb INTERFACE
  STB_IMAGE_IMPLEMENTATION
  STB_IMAGE_WRITE_IMPLEMENTATION
)
target_include_directories(stb SYSTEM INTERFACE stb)

# tinyexr
add_library(tinyexr INTERFACE)
target_compile_definitions(tinyexr INTERFACE TINYEXR_IMPLEMENTATION)
target_include_directories(tinyexr SYSTEM INTERFACE tinyexr)

# tinyobjloader
add_library(tinyobjloader INTERFACE)
target_compile_definitions(tinyobjloader INTERFACE TINYOBJLOADER_IMPLEMENTATION)
target_include_directories(tinyobjloader SYSTEM INTERFACE tinyobjloader)