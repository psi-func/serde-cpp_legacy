# GoogleTest for unit testing
find_package(GTest REQUIRED)
include(GoogleTest)

set(SERDE_YAML_TEST_SOURCES advanced.cpp std.cpp types.cpp
                            errors.cpp builtin.cpp)

add_executable(serde_yaml_test ${SERDE_YAML_TEST_SOURCES})

target_link_libraries(
  serde_yaml_test PRIVATE serde::serde_yaml GTest::gmock_main GTest::gmock
                          GTest::gtest)
