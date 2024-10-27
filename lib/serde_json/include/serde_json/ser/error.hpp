#pragma once

#include <string>

namespace serde_json {

struct SerError {
    static auto custom(std::string msg) -> SerError { return SerError{ .msg = std::move(msg) }; }

    std::string msg;
};

}  // namespace serde_json