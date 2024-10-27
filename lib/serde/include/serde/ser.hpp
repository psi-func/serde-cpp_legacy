#pragma once

#include <expected>
#include <concepts>

#include "ser/builtin.hpp"
#include "ser/serializer.hpp"

namespace serde::ser {

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
    constexpr auto operator()(T&& obj, S&& serializer) const requires Serializer<std::remove_cvref_t<S>>
    // requires Serializer<S>
    {
        if constexpr (IsSerializable<T, S>) {
            // ADL
            return serialize(std::forward<T>(obj), std::forward<S>(serializer));
        }
        else {
            // default impl
            return serde::ser::builtin::serialize(std::forward<T>(obj),
                                                  std::forward<S>(serializer));
        }
    }
};

}  // namespace serialize_impl

inline constexpr serialize_impl::fn serialize;

}  // namespace serde::ser
