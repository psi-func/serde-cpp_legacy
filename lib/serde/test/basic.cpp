#include "serde/ser.hpp"

#include <iostream>

struct serializer {
    using Ok = void;
    struct Error {
        static Error custom(std::string msg) { return Error{ .err_ = std::move(msg) }; }
        std::string err_;
    };
    auto serialize_i32(int a) -> std::expected<Ok, Error>
    {
        std::cout << a << '\n';
        return {};
    };
};

int main()
{
    auto result = serde::ser::serialize(12, serializer{});

    return 0;
}