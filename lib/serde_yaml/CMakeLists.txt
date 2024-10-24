cmake_minimum_required(VERSION 3.21)
project(serde_yaml VERSION 0.1.0)

option(SERDE_YAML_BUILD_TESTS "Build yaml test" ON)

# ##############################################################################
# Dependencies
# ##############################################################################
# RapidYaml for parsing and emitting YAML
find_package(ryml 0.5 REQUIRED)

# ##############################################################################
# serde_yaml
# ##############################################################################
add_library(serde_yaml STATIC)
target_sources(serde_yaml PRIVATE src/serializer_yaml.cpp
                                  src/deserializer_yaml.cpp)
target_include_directories(
  serde_yaml PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                    $<INSTALL_INTERFACE:include>)
target_link_libraries(
  serde_yaml
  PUBLIC serde::serde
  PRIVATE $<BUILD_INTERFACE:ryml::ryml>)

add_library(serde::serde_yaml ALIAS serde_yaml)

install(TARGETS serde_yaml EXPORT serde_cppTargets)
install(DIRECTORY include/serde_yaml DESTINATION include)

# ##############################################################################
# Tests
# ##############################################################################
if(SERDE_YAML_BUILD_TESTS)
  add_subdirectory(test)
endif()
