# ##############################################################################
# Configuration
# ##############################################################################
# Default build type to Debug, for release use `cmake -DCMAKE_BUILD_TYPE=Release
# ..`
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE
      "Debug"
      CACHE STRING "Build type default to Debug" FORCE)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# ##############################################################################
# Sub-directories
# ##############################################################################
add_subdirectory(serde)

if(SERDE_BUILD_YAML)
  add_subdirectory(serde_yaml)
endif()

if(SERDE_BUILD_JSON)
  add_subdirectory(serde_json)
endif()

# ##############################################################################
# Package Configuration
# ##############################################################################
# Helpers functions for creating config files that can be included by other
# projects
include(CMakePackageConfigHelpers)

# Create config package
configure_package_config_file(
  "Config.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/serde_cppConfig.cmake"
  INSTALL_DESTINATION "lib/cmake/serde_cpp")
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/serde_cppConfigVersion.cmake"
  COMPATIBILITY SameMajorVersion)

# Install config package
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/serde_cppConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/serde_cppConfigVersion.cmake"
        DESTINATION "lib/cmake/serde_cpp")
install(
  EXPORT "serde_cppTargets"
  NAMESPACE "serde_cpp::"
  DESTINATION "lib/cmake/serde_cpp")
