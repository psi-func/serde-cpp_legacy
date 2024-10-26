#pragma once

#include <string>

namespace types {

enum class Number {
  One,
  Two,
  Three,
};

struct Student  {
  std::string name;
  int age;
};

struct Point {
  int x;
  int y;
  Number num;
};

} // namespace types