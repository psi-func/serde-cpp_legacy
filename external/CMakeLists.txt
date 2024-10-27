include(FetchContent)

FetchContent_Declare(
  gtest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
  OVERRIDE_FIND_PACKAGE)

FetchContent_Declare(
  cppast
  GIT_REPOSITORY https://github.com/standardese/cppast.git
  GIT_TAG 5329e377ab9b9ab9309e9641f3fcda04366a449a # 14 JUL 2024
  OVERRIDE_FIND_PACKAGE)

FetchContent_Declare(
  cxxopts
  GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
  GIT_TAG v3.2.1
  OVERRIDE_FIND_PACKAGE)

FetchContent_Declare(
  ryml
  GIT_REPOSITORY https://github.com/biojppm/rapidyaml.git
  GIT_TAG v0.7.2
  OVERRIDE_FIND_PACKAGE)

FetchContent_Declare(
  glaze
  GIT_REPOSITORY https://github.com/stephenberry/glaze.git
  GIT_TAG v3.6.2
  OVERRIDE_FIND_PACKAGE)


FetchContent_MakeAvailable(ryml glaze cppast gtest cxxopts)
