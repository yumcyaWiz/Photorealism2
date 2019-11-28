cmake_minimum_required(VERSION 3.12..3.15)

project(Photorealism2 LANGUAGES C CXX)

# build type
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to Release as none was specified.")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
endif()

# compile definitions(MSVC)
add_definitions(
  -D_ENABLE_EXTENDED_ALIGNED_STORAGE
)

#Photorealism2 Viewer
add_subdirectory(app)
#Photorealism2 Library
add_subdirectory(lib/prl2)