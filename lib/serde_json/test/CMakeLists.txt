# GoogleTest for unit testing
find_package(GTest REQUIRED)
include(GoogleTest)

set(SERDE_JSON_TEST_SOURCES builtin.cpp advanced.cpp types.cpp)

add_executable(serde_json_test ${SERDE_JSON_TEST_SOURCES})

target_link_libraries(
  serde_json_test PRIVATE glaze::glaze serde::serde_json GTest::gmock_main GTest::gmock
                          GTest::gtest)
