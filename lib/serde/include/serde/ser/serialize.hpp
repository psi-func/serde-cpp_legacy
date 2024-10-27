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

}  // namespace serde::ser
