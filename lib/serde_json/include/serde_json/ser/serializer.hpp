#pragma once

#include <serde/ser.hpp>
#include "error.hpp"

#include <glaze/glaze.hpp>

namespace serde_json {

class Serializer {
   public:
    using Ok = void;
    using Error = SerError;

    // Scalars ///////////////////////////////////////////////////////////////////
    auto serialize_bool(bool) -> std::expected<Ok, Error> {}

    auto serialize_i8(int8_t v) -> std::expected<Ok, Error> {}
    auto serialize_i16(int16_t v) -> std::expected<Ok, Error> {}
    auto serialize_i32(int32_t v) -> std::expected<Ok, Error> {}
    auto serialize_i64(int64_t v) -> std::expected<Ok, Error> {}

    auto serialize_u8(uint8_t v) -> std::expected<Ok, Error> {}
    auto serialize_u16(uint16_t v) -> std::expected<Ok, Error> {}
    auto serialize_u32(uint32_t v) -> std::expected<Ok, Error> {}
    auto serialize_u64(uint64_t v) -> std::expected<Ok, Error> {}

    auto serialize_float(float v) -> std::expected<Ok, Error> {}
    auto serialize_double(double v) -> std::expected<Ok, Error> {}

    auto serialize_char(char v) -> std::expected<Ok, Error> {}
    auto serialize_uchar(unsigned char v) -> std::expected<Ok, Error> {}

    auto serialize_cstr(const char* v) -> std::expected<Ok, Error> {}
    auto serialize_bytes(const void* v, std::size_t sz) -> std::expected<Ok, Error> {}

    // Optional //////////////////////////////////////////////////////////////////
    auto serialize_none() -> std::expected<Ok, Error> {}

    // Sequence //////////////////////////////////////////////////////////////////
    auto serialize_seq_begin() -> std::expected<Ok, Error> {}
    auto serialize_seq_end() -> std::expected<Ok, Error> {}

    // Map ///////////////////////////////////////////////////////////////////////
    auto serialize_map_begin() -> std::expected<Ok, Error> {}
    auto serialize_map_end() -> std::expected<Ok, Error> {}

    auto serialize_map_key_begin() -> std::expected<Ok, Error> {}
    auto serialize_map_key_end() -> std::expected<Ok, Error> {}

    auto serialize_map_value_begin() -> std::expected<Ok, Error> {}
    auto serialize_map_value_end() -> std::expected<Ok, Error> {}

    // Struct ////////////////////////////////////////////////////////////////////
    auto serialize_struct_begin() -> std::expected<Ok, Error> {}
    auto serialize_struct_end() -> std::expected<Ok, Error> {}
    auto serialize_struct_field_begin(const char* field) -> std::expected<Ok, Error> {}
    auto serialize_struct_field_end() -> std::expected<Ok, Error> {}

    // utility
    std::string emit()
    {
        auto res = glz::write<glz::opts{ .prettify = true }>(val);
        return res.value();
    }

   private:
    glz::json_t val;
};

}  // namespace serde_json