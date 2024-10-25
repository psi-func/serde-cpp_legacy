#pragma once

#include <string>
#include <expected>

#include <serde/ser.h>
#include <serde/error.h>

#include "detail/ser_detail.hpp"

///////////////////////////////////////////////////////////////////////////////
// Serde JSON
///////////////////////////////////////////////////////////////////////////////
namespace serde_json {

/// JSON Serializer function from T to json string
template<typename T>
auto to_string(T&& obj) -> std::expected<std::string, serde::Error>
{
    auto ser = detail::SerializerNew();
    ser->serialize(std::forward<T>(obj));
    return detail::SerializerOutput(ser.get());
}

}  // namespace serde_json
