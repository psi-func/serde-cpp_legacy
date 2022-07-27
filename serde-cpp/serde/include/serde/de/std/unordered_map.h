#pragma once

#include <unordered_map>

#include "../deserialize.h"
#include "../deserializer.h"

namespace serde {

template<>
struct Deserialize<std::unordered_map> {
  template<typename Key, typename Value, typename... U>
  static void deserialize(Deserializer& de, std::unordered_map<Key, Value, U...>& map) {
    size_t size = 0;
    map.clear();
    de.deserialize_map_begin();
    de.deserialize_map_size(size);
    for (size_t i = 0; i < size; i++) {
      Key key; Value value;
      de.deserialize_map_entry(key, value);
      map.insert(std::move(key), std::move(value));
    }
    de.deserialize_map_end();
  }
};

template<>
struct Deserialize<std::unordered_multimap> {
  template<typename Key, typename Value, typename... U>
  static void deserialize(Deserializer& de, std::unordered_multimap<Key, Value, U...>& multimap) {
    size_t size = 0;
    multimap.clear();
    de.deserialize_map_begin();
    de.deserialize_map_size(size);
    for (size_t i = 0; i < size; i++) {
      Key key; Value value;
      de.deserialize_map_entry(key, value);
      multimap.insert(std::move(key), std::move(value));
    }
    de.deserialize_map_end();
  }
};

} // namespace serde

