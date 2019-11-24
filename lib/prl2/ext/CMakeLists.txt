# HosekWilkie
add_library(hosek STATIC
 HosekWilkie/ArHosekSkyModel.c 
)
target_include_directories(hosek PUBLIC
  HosekWilkie
)

# ThreadPool
add_library(pool INTERFACE)
target_include_directories(pool INTERFACE
  ThreadPool
)