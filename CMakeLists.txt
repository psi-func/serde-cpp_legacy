cmake_minimum_required(VERSION 3.16)
project(serde VERSION 0.1.0)

option(SERDE_BUILD_GENERATOR "Build serde generator" OFF)
option(SERDE_BUILD_TEST "Build tests" OFF)
option(SERDE_BUILD_YAML "Build yaml serializer" ON)

add_subdirectory(lib)
