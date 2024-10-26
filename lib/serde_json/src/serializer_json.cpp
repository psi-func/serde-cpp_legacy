#include <serde_json/detail/ser_detail.hpp>

#include <stack>
#include <cctype>

#include <glaze/glaze.hpp>
#include <glaze/base64/base64.hpp>

////////////////////////////////////////////////////////////////////////////////
// Serde JSON
////////////////////////////////////////////////////////////////////////////////
namespace serde_json {

class JsonSerializer final : public serde::Serializer {
   public:
    JsonSerializer() {}

    //////////////////////////////////////////////////////////////////////////////
    // Serializer interface
    //////////////////////////////////////////////////////////////////////////////

    // Scalars ///////////////////////////////////////////////////////////////////
    void serialize_bool(bool v) override { serialize_scalar(v); }
    void serialize_i8(int8_t v) override { serialize_scalar(v); }
    void serialize_u8(uint8_t v) override { serialize_scalar(v); }
    void serialize_i16(int16_t v) override { serialize_scalar(v); }
    void serialize_u16(uint16_t v) override { serialize_scalar(v); }
    void serialize_i32(int32_t v) override { serialize_scalar(v); }
    void serialize_u32(uint32_t v) override { serialize_scalar(v); }
    void serialize_i64(int64_t v) override { serialize_scalar(v); }
    void serialize_u64(uint64_t v) override { serialize_scalar(v); }
    void serialize_float(float v) override { serialize_scalar(v); }
    void serialize_double(double v) override { serialize_scalar(v); }
    void serialize_char(char v) override
    {
        if (std::isprint(v)) {
            serialize_scalar(std::string(1, v));
        }
        else {
            serialize_scalar(v);
        }
    }
    void serialize_uchar(unsigned char v) override { serialize_scalar(v); }
    void serialize_cstr(const char* v) override { serialize_scalar(v); }
    void serialize_bytes(const void* val, size_t len) override
    {
        serialize_scalar(
            glz::write_base64(std::string_view(reinterpret_cast<const char*>(val), len)));
    }

    // Optional //////////////////////////////////////////////////////////////////
    void serialize_none() override { serialize_scalar(nullptr); }

    // Sequence //////////////////////////////////////////////////////////////////
    void serialize_seq_begin() override
    {
        if (err_.has_value()) {
            return;
        }

        if (await_key) {
            err_.emplace(serde::Error{ .kind = serde::Error::Kind::Invalid,
                                       .text = "Expected string key, not seq" });
            return;
        }
        stack_.push(glz::json_t(glz::json_t::array_t()));
    }

    void serialize_seq_end() override { complex_end(); }

    // Map ///////////////////////////////////////////////////////////////////////
    void serialize_map_begin() override
    {
        if (err_.has_value()) {
            return;
        }
        stack_.push(glz::json_t(glz::json_t::object_t()));
    }

    void serialize_map_end() override { complex_end(); }

    void serialize_map_key_begin() override { await_key = true; }

    void serialize_map_key_end() override { await_key = false; }

    void serialize_map_value_begin() override {}

    void serialize_map_value_end() override {}

    // Struct ////////////////////////////////////////////////////////////////////
    void serialize_struct_begin() override { serialize_map_begin(); }
    void serialize_struct_end() override { serialize_map_end(); }

    void serialize_struct_field_begin(const char* name) override
    {
        serialize_map_key_begin();
        serialize(name);
        serialize_map_key_end();
        serialize_map_value_begin();
    }
    void serialize_struct_field_end() override { serialize_map_value_end(); }

    //////////////////////////////////////////////////////////////////////////////
    // Serialization Utils
    //////////////////////////////////////////////////////////////////////////////

    template<typename T>
    void serialize_scalar(T&& val)
    {
        if (err_.has_value()) {
            return;
        }

        if (stack_.empty()) {
            stack_.push(glz::json_t(val));
            return;
        }

        auto& top = stack_.top();
        if (top.is_object()) {

            if (await_key) {
                if constexpr (std::is_convertible_v<T, std::string>) {
                    keys_.push(std::string(val));
                }
                else if constexpr (std::is_same_v<std::nullptr_t, T>) {
                    err_.emplace(serde::Error{ .kind = serde::Error::Kind::Invalid,
                                               .text = "Cannot use null as key in JSON" });
                    return;
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
        // TODO: cannot go here??
    }

    void complex_end()
    {
        if (stack_.size() == 1) {
            return;
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
        }
    }

    std::expected<std::string, serde::Error> emit() const
    {
        if (err_) {
            return std::unexpected(err_.value());
        }
        auto res = glz::write<glz::opts{ .prettify = true }>(stack_.top());
        if (!res) [[unlikely]] {
            return std::unexpected(
                serde::Error{ .kind = serde::Error::Kind::Invalid,
                              .text = std::string(res.error().custom_error_message) });
        }

        return res.value();
    }

   private:
    bool await_key;
    std::optional<serde::Error> err_;
    std::stack<std::string> keys_;
    std::stack<glz::json_t> stack_;
};

namespace detail {

auto SerializerNew() -> std::unique_ptr<serde::Serializer>
{
    return std::make_unique<JsonSerializer>();
}

auto SerializerOutput(serde::Serializer* ser) -> std::expected<std::string, serde::Error>
{
    auto jsonser = static_cast<JsonSerializer*>(ser);
    return jsonser->emit();
}

}  // namespace detail

}  // namespace serde_json