#pragma once

#include <array>
#include "../serialize.h"
#include "../serializer.h"

namespace serde {

template<>
struct SerializeTN<std::array> {
  template<typename T, auto N>
  static void serialize(Serializer& ser, const std::array<T, N>& arr) {
    ser.serialize_seq_begin();
    for (auto& e : arr)
      ser.serialize(e);
    ser.serialize_seq_end();
  }
};

} // namespace serde
