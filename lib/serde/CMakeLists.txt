#########################################################################################
# serde
#########################################################################################
add_library(serde INTERFACE)
target_include_directories(serde INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(serde INTERFACE cxx_std_23)

add_library(serde::serde ALIAS serde)

install(TARGETS serde EXPORT serde_cppTargets)
install(DIRECTORY include/serde DESTINATION include)

add_subdirectory(test)

