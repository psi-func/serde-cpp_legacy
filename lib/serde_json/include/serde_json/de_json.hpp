#pragma once

#include <string>
#include <expected>

#include <serde/de.h>
#include <serde/error.h>
#include "detail/de_detail.hpp"

///////////////////////////////////////////////////////////////////////////////
// Serde JSON
///////////////////////////////////////////////////////////////////////////////
namespace serde_json {

/// JSON Deserializer function from JSON string to T
template<typename T>
auto from_str(std::string&& str) -> std::expected<T, serde::Error>
{
    auto de = detail::DeserializerNew(std::move(str));
    std::ignore = detail::DeserializerParse(de.get());
    T obj{};
    de->deserialize(obj);
    return std::move(obj);
}

}  // namespace serde_json