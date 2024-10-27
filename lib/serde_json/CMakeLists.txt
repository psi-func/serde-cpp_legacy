cmake_minimum_required(VERSION 3.16)
project(serde_json VERSION 0.1.0)

option(SERDE_JSON_BUILD_TESTS "Build JSON serializer tests" ON)

# ##############################################################################
# Dependencies
# ##############################################################################
# Glaze for parsing and emitting json
find_package(glaze 3.6 REQUIRED)

# ##############################################################################
# serde_json
# ##############################################################################
add_library(serde_json INTERFACE)

target_compile_features(serde_json INTERFACE cxx_std_23)

target_include_directories(
  serde_json INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                    $<INSTALL_INTERFACE:include>)

target_link_libraries(
  serde_json
  INTERFACE serde::serde
  INTERFACE $<BUILD_INTERFACE:glaze::glaze>)

add_library(serde::serde_json ALIAS serde_json)

install(TARGETS serde_json EXPORT serde_cppTargets)
install(DIRECTORY include/serde_json DESTINATION include)

# ##############################################################################
# Tests
# ##############################################################################
if(SERDE_JSON_BUILD_TESTS)
  add_subdirectory(test)
endif()