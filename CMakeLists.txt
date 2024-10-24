cmake_minimum_required(VERSION 3.16)
project(serde-cpp)

option(SERDE_BUILD_GENERATOR "Build serde generator" OFF)
option(SERDE_BUILD_TEST "Build tests" OFF)
option(SERDE_BUILD_YAML "Build yaml serializer" OFF)

add_subdirectory(serde-cpp)
