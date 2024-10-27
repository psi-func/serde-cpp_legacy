add_executable(test-serde basic.cpp)

target_link_libraries(test-serde PUBLIC serde::serde)