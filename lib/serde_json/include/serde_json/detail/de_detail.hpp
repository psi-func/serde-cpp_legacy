#pragma once

#include <memory>
#include <string>
#include <expected>

#include <serde/error.h>
#include <serde/de/deserializer.h>

///////////////////////////////////////////////////////////////////////////////
// Serde JSON detail
///////////////////////////////////////////////////////////////////////////////
namespace serde_json::detail {

auto DeserializerNew(std::string&& str) -> std::unique_ptr<serde::Deserializer>;
auto DeserializerParse(serde::Deserializer* de) -> std::expected<void, serde::Error>;

}  // namespace serde_json::detail
