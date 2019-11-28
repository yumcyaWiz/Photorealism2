cmake_minimum_required(VERSION 3.12..3.15)

project(Photorealism2 LANGUAGES C CXX)

#Photorealism2
add_subdirectory(app)
add_subdirectory(lib/prl2)

# build type
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to Release as none was specified.")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
endif()