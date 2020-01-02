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
  STB_IMAGE_WRITE_IMPLEMENTATION
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

# tbb
# include(tbb/cmake/TBBBuild.cmake)
# tbb_build(TBB_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/tbb CONFIG_DIR TBB_DIR)
# find_package(TBB REQUIRED)

# Intel Open Image Denoise
# set(OIDN_STATIC_LIB ON CACHE BOOL "" FORCE)
# add_subdirectory(oidn)
find_package(OpenImageDenoise REQUIRED)

# Embree3
find_package(embree 3.6.1 REQUIRED)
target_include_directories(prl2 PUBLIC ${EMBREE_INCLUDE_DIRS})
target_link_libraries(prl2 PUBLIC ${EMBREE_LIBRARY})

# tinyobjloader
add_library(tinyobjloader INTERFACE)
target_compile_definitions(tinyobjloader INTERFACE TINYOBJLOADER_IMPLEMENTATION)
target_include_directories(tinyobjloader INTERFACE tinyobjloader)