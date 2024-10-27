#pragma once

#include <expected>
#include <string>

#include "ser/serializer.hpp"

namespace serde_json {

template<typename T>
auto to_string(T&& val) -> std::expected<std::string, SerError>
{
    serde_json::Serializer ser{};
    auto res = serde::ser::serialize(val, ser);
    if (!res) {
        return std::unexpected(res.error());
    }
    return ser.emit();
}

}  // namespace serde_json