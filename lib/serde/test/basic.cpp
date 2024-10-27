#include "serde/ser.hpp"

#include <iostream>

struct serializer : serde::ser::SerializerDef {
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

    auto serialize_cstr(const char* str) -> std::expected<Ok, Error>
    {
        std::cout << str << '\n';
        return {};
    }
};

int main()
{
    auto result = serde::ser::serialize("Hello world", serializer{});

    return 0;
}