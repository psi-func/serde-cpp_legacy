#pragma once
// IWYU pragma: private, include "ser_json.hpp"

#include <memory>
#include <string>
#include <expected>

#include <serde/error.h>
#include <serde/ser/serializer.h>

///////////////////////////////////////////////////////////////////////////////
// Serde JSON detail
///////////////////////////////////////////////////////////////////////////////
namespace serde_json::detail {

auto SerializerNew() -> std::unique_ptr<serde::Serializer>;
auto SerializerOutput(serde::Serializer* ser) -> std::expected<std::string, serde::Error>;

}  // namespace serde_json::detail