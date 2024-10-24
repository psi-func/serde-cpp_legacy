include(FetchContent)

FetchContent_Declare(
  gtest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
  OVERRIDE_FIND_PACKAGE)

FetchContent_Declare(
  cppast
  GIT_REPOSITORY https://github.com/standardese/cppast.git
  GIT_TAG v0.1.0
  OVERRIDE_FIND_PACKAGE)

FetchContent_Declare(
  ryml
  GIT_REPOSITORY https://github.com/biojppm/rapidyaml.git
  GIT_TAG v0.7.2
  OVERRIDE_FIND_PACKAGE)

FetchContent_MakeAvailable(ryml cppast gtest)
