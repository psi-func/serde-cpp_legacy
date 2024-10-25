#include <serde_json/detail/ser_detail.hpp>

#include <stack>
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
    void serialize_char(char v) override { serialize_scalar(v); }
    void serialize_uchar(unsigned char v) override { serialize_scalar(v); }
    void serialize_cstr(const char* v) override { serialize_scalar(v); }
    void serialize_bytes(const void* val, size_t len) override
    {
        serialize_scalar(
            glz::read_base64(std::string_view(reinterpret_cast<const char*>(val), len)));
    }

    // Optional //////////////////////////////////////////////////////////////////
    void serialize_none() override
    {
        using namespace std::string_literals;
        serialize_scalar(glz::json_t(nullptr));
    }

    // Sequence //////////////////////////////////////////////////////////////////
    void serialize_seq_begin() override { stack.push(glz::json_t(glz::json_t::array_t())); }

    void serialize_seq_end() override
    {
        if (stack.size() == 1) {
            return;
        }
        else {
            auto json_arr = std::move(stack.top());
            stack.pop();

            auto& prev_obj = stack.top();

            if (prev_obj.is_object()) {
                // TODO:
            }
            else if (prev_obj.is_array()) {
                prev_obj.get_array().emplace_back(std::move(json_arr));
            }
        }
    }

    // Map ///////////////////////////////////////////////////////////////////////
    void serialize_map_begin() override { stack.push(glz::json_t(glz::json_t::object_t())); }

    void serialize_map_end() override {}

    void serialize_map_key_begin() override {}

    void serialize_map_key_end() override {}

    void serialize_map_value_begin() override {}

    void serialize_map_value_end() override {}

    // Struct ////////////////////////////////////////////////////////////////////
    void serialize_struct_begin() override {}
    void serialize_struct_end() override {}

    void serialize_struct_field_begin(const char* name) override {}
    void serialize_struct_field_end() override {}

    //////////////////////////////////////////////////////////////////////////////
    // Serialization Utils
    //////////////////////////////////////////////////////////////////////////////

    template<typename T>
    void serialize_scalar(T&& val)
    {
        if (stack.empty()) {
            stack.push(glz::json_t(val));
            return;
        }

        auto& top = stack.top();
        if (top.is_object()) {
            top.get_object();
        }
        else if (top.is_array()) {
            top.get_array().emplace_back(glz::json_t(val));
        }
    }

    std::string emit() const { return stack.top().dump().value(); }

   private:
    std::stack<glz::json_t> stack;
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