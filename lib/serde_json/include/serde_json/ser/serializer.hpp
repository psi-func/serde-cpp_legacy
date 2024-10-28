#pragma once

#include <serde/ser.hpp>
#include "error.hpp"

#include <glaze/glaze.hpp>
#include <glaze/base64/base64.hpp>

namespace serde_json {

class Serializer {
   public:
    using Ok = void;
    using Error = SerError;

    // Scalars ///////////////////////////////////////////////////////////////////
    auto serialize_bool(bool v) -> std::expected<Ok, Error> { return serialize_scalar(v); }

    auto serialize_i8(int8_t v) -> std::expected<Ok, Error> { return serialize_scalar(v); }
    auto serialize_i16(int16_t v) -> std::expected<Ok, Error> { return serialize_scalar(v); }
    auto serialize_i32(int32_t v) -> std::expected<Ok, Error> { return serialize_scalar(v); }
    auto serialize_i64(int64_t v) -> std::expected<Ok, Error> { return serialize_scalar(v); }

    auto serialize_u8(uint8_t v) -> std::expected<Ok, Error> { return serialize_scalar(v); }
    auto serialize_u16(uint16_t v) -> std::expected<Ok, Error> { return serialize_scalar(v); }
    auto serialize_u32(uint32_t v) -> std::expected<Ok, Error> { return serialize_scalar(v); }
    auto serialize_u64(uint64_t v) -> std::expected<Ok, Error> { return serialize_scalar(v); }

    auto serialize_float(float v) -> std::expected<Ok, Error> { return serialize_scalar(v); }
    auto serialize_double(double v) -> std::expected<Ok, Error> { return serialize_scalar(v); }

    auto serialize_char(char v) -> std::expected<Ok, Error>
    {
        return serialize_scalar(std::string(1, v));
    }
    auto serialize_uchar(unsigned char v) -> std::expected<Ok, Error>
    {
        return serialize_scalar(v);
    }

    auto serialize_cstr(const char* v) -> std::expected<Ok, Error> { return serialize_scalar(v); }
    auto serialize_bytes(const void* v, std::size_t sz) -> std::expected<Ok, Error>
    {
        return serialize_scalar(
            glz::write_base64(std::string_view(reinterpret_cast<const char*>(v), sz)));
    }

    // Optional //////////////////////////////////////////////////////////////////
    auto serialize_none() -> std::expected<Ok, Error> { return serialize_scalar(nullptr); }

    // Sequence //////////////////////////////////////////////////////////////////
    auto serialize_seq_begin() -> std::expected<Ok, Error>
    {
        if (await_key_) {
            return std::unexpected(Error::custom("Expected string key, not seq"));
        }
        stack_.push(glz::json_t(glz::json_t::array_t()));
        return {};
    }

    auto serialize_seq_end() -> std::expected<Ok, Error> { return complex_end(); }

    // Map ///////////////////////////////////////////////////////////////////////
    auto serialize_map_begin() -> std::expected<Ok, Error>
    {
        stack_.push(glz::json_t(glz::json_t::object_t()));
        return {};
    }

    auto serialize_map_end() -> std::expected<Ok, Error> { return complex_end(); }

    auto serialize_map_key_begin() -> std::expected<Ok, Error>
    {
        await_key_ = true;
        return {};
    }
    auto serialize_map_key_end() -> std::expected<Ok, Error>
    {
        await_key_ = false;
        return {};
    }

    auto serialize_map_value_begin() -> std::expected<Ok, Error> { return {}; }
    auto serialize_map_value_end() -> std::expected<Ok, Error> { return {}; }

    // Struct ////////////////////////////////////////////////////////////////////
    auto serialize_struct_begin() -> std::expected<Ok, Error> { return serialize_map_begin(); }
    auto serialize_struct_end() -> std::expected<Ok, Error> { return serialize_map_end(); }

    auto serialize_struct_field_begin(const char* field) -> std::expected<Ok, Error>
    {
        serialize_map_key_begin();
        auto res = serde::ser::serialize(field, *this);
        if (!res) {
            return res;
        }
        serialize_map_key_end();
        serialize_map_value_begin();
        return {};
    }

    auto serialize_struct_field_end() -> std::expected<Ok, Error>
    {
        serialize_map_value_end();
        return {};
    }

    // utility
    template<typename T>
    auto serialize_scalar(T&& val) -> std::expected<Ok, Error>
    {
        if (stack_.empty()) {
            stack_.push(glz::json_t(val));
            return {};
        }

        auto& top = stack_.top();
        if (top.is_object()) {

            if (await_key_) {
                if constexpr (std::is_convertible_v<T, std::string>) {
                    keys_.push(std::string(val));
                }
                else if constexpr (std::is_same_v<std::nullptr_t, T>) {
                    return std::unexpected(Error::custom("cannot use null as key in JSON"));
                }
                else {
                    keys_.push(std::to_string(val));
                }
            }
            else {
                top.get_object().emplace(std::move(keys_.top()), glz::json_t(val));
                keys_.pop();
            }
        }
        else if (top.is_array()) {
            top.get_array().emplace_back(glz::json_t(val));
        }
        return {};
    }

    auto complex_end() -> std::expected<Ok, Error>
    {
        if (stack_.size() == 1) {
            return {};
        }
        else {
            auto json_arr = std::move(stack_.top());
            stack_.pop();

            auto& prev_obj = stack_.top();

            if (prev_obj.is_object()) {
                // TODO: add check if keys is empty (impossible in correct case)
                auto key = std::move(keys_.top());
                keys_.pop();
                prev_obj.get_object().emplace(std::move(key), std::move(json_arr));
            }
            else if (prev_obj.is_array()) {
                prev_obj.get_array().emplace_back(std::move(json_arr));
            }
            return {};
        }
    }

    std::string emit() const
    {
        auto res = glz::write<glz::opts{ .prettify = true }>(stack_.top());
        return res.value();
    }

   private:
    bool await_key_;
    std::stack<std::string> keys_;
    std::stack<glz::json_t> stack_;
};

}  // namespace serde_json