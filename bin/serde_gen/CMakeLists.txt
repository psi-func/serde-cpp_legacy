# Default compilation settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -pg -fsanitize=undefined -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG -Wall")

# ##############################################################################
# Dependencies
# ##############################################################################
find_package(cxxopts REQUIRED)

# CppAST to parse and work with the C++ AST
find_package(cppast REQUIRED)

# ##############################################################################
# serde_gen
# ##############################################################################
add_executable(serde_gen src/main.cpp src/init.cpp src/common.cpp
                         src/generate.cpp)
add_executable(serde_cpp::serde_gen ALIAS serde_gen)
target_link_libraries(serde_gen PRIVATE serde cppast cxxopts::cxxopts)

# ##############################################################################
# Installs
# ##############################################################################
install(TARGETS serde_gen EXPORT serde_cppTargets)
install(FILES cmake/serde_cpp.cmake DESTINATION lib/cmake/serde_cpp)

# ##############################################################################
# Scripts
# ##############################################################################
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(serde_cpp)

# ##############################################################################
# Tests
# ##############################################################################
if(SERDE_BUILD_TEST)
  add_subdirectory(test)
endif()
