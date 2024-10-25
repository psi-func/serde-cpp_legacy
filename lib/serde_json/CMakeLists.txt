cmake_minimum_required(VERSION 3.16)
project(serde_json VERSION 0.1.0)

# ##############################################################################
# Dependencies
# ##############################################################################
# Glaze for parsing and emitting json
find_package(glaze 3.6 REQUIRED)

# ##############################################################################
# serde_json
# ##############################################################################
set(SERDE_JSON_SOURCES src/test.cpp)

add_library(serde_json STATIC ${SERDE_JSON_SOURCES})

target_include_directories(
  serde_json PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                    $<INSTALL_INTERFACE:include>)

target_link_libraries(
  serde_json
  PUBLIC serde::serde
  PRIVATE $<BUILD_INTERFACE:glaze::glaze>)

add_library(serde::serde_json ALIAS serde_json)

install(TARGETS serde_json EXPORT serde_cppTargets)
install(DIRECTORY include/serde_json DESTINATION include)

# ##############################################################################
# Tests
# ##############################################################################
if(SERDE_JSON_BUILD_TESTS)
  add_subdirectory(test)
endif()
