#pragma once

#include <utility>
#include <concepts>
#include <expected>

namespace serde::ser {

template<typename T, typename S>
concept Serialize = requires(T&& t, S&& serializer) {
    {
        serialize(std::forward<T>(t), std::forward<S>(serializer))
    } -> std::same_as<std::expected<typename S::Ok, typename S::Error>>;
};

#if 0


// forward-declaration
class Serializer;


// Serialization for simple types should ONLY specialize function `serialize` below.
// The specialization for simple types can be in a translation unit (.cpp).
template<typename T>
void serialize(Serializer& ser, const T& val);

// Serialization for simple types but with the perks of specialization only on instantiation.
// Serialize is not defined here because we check for specialization in constexpr evaluation.
template<typename T, typename = void>
struct Serialize;
// {
//   static void serialize(Serializer& ser, const T& val);
// };


// Serialization for template types should ONLY specialize struct Serialize::serialize below.
// The specialization for template types must be available concretely before the Serializer is invoked!
template<template<typename...> typename T>
struct SerializeT {
  template<typename... U>
  static void serialize(Serializer& ser, const T<U...>& val);
};
// TODO: evaluate removing definition of Serialize (and variations) for breaking at compile time instead of link time

// Serialization for template types, forwards to struct Serialize::serialize.
template<template<typename...> typename T, typename... U>
inline void serialize(Serializer& ser, const T<U...>& val) {
  SerializeT<T>::template serialize<U...>(ser, val);
}


// Serialization for template types with only integral parameter, e.g. std::bitset.
template<template<auto...> typename T>
struct SerializeN {
  template<auto... N>
  static void serialize(Serializer& ser, const T<N...>& val);
};

// Serialization for template types with only integral parameter, e.g. std::bitset
// forwards to struct SerializeN::serialize.
template<template<auto...> typename T, auto... N>
inline void serialize(Serializer& ser, const T<N...>& val) {
  SerializeN<T>::template serialize<N...>(ser, val);
}


// Serialization for template types with typename and integral parameter, e.g. std::array.
template<template<typename, auto, auto...> typename T>
struct SerializeTN {
  template<typename U, auto... N>
  static void serialize(Serializer& ser, const T<U, N...>& val);
};

// Serialization for template types with typename and integral parameter, e.g. std::array.
// forwards to struct SerializeTN::serialize.
template<template<typename, auto, auto...> typename T, typename U, auto N, auto... M>
inline void serialize(Serializer& ser, const T<U, N, M...>& val) {
  SerializeTN<T>::template serialize<U, N, M...>(ser, val);
}


// Serialization for string literals, builtin implementation!
template<size_t N>
void serialize(Serializer& ser, const char (&val)[N]);
#endif

}  // namespace serde::ser
