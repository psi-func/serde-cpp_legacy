#pragma once

#include <expected>
#include <concepts>

#include "ser/serializer.hpp"

namespace serde::ser {

// namespace builtin {
// template<typename T>
// struct Serialize;
//
// template<>
// struct Serialize<int> {
//     template<typename S>
//     static auto serialize(int a, S&& ser) -> std::expected<typename S::Ok, typename S::Error>
//     {
//         return {};
//     }
// };
// }  // namespace builtin

namespace serialize_impl {

// non-ADL lookup block (poison pill)
void serialize();  // undef

template<typename T, typename S>
inline constexpr bool IsSerializable = requires(T&& t, S&& serializer) {
    {
        serialize(std::forward<T>(t), std::forward<S>(serializer))
    } -> std::same_as<std::expected<typename S::Ok, typename S::Error>>;
};

struct fn {

    template<typename T, typename S>
    constexpr auto operator()(T&& obj, S&& serializer) const
    // requires Serializer<S>
    {
        if constexpr (IsSerializable<T, S>) {
            // ADL
            return serialize(std::forward<T>(obj), std::forward<S>(serializer));
        }
        else {
            // default impl
            return ::serde::ser::serialize(std::forward<T>(obj), std::forward<S>(serializer));
        }
    }
};

}  // namespace serialize_impl

inline constexpr serialize_impl::fn serialize;

}  // namespace serde::ser
