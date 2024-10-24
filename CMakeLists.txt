cmake_minimum_required(VERSION 3.24)
project(serde VERSION 0.1.0)

option(SERDE_BUILD_GENERATOR "Build serde generator" OFF)
option(SERDE_BUILD_EXTERNAL "Build external libraries" OFF)
option(SERDE_BUILD_TEST "Build tests" OFF)
option(SERDE_BUILD_YAML "Build yaml serializer" ON)

if(SERDE_BUILD_EXTERNAL)
  add_subdirectory(external)
endif()

add_subdirectory(lib)

if(SERDE_BUILD_GENERATOR)
  add_subdirectory(bin/serde_gen)
endif()
