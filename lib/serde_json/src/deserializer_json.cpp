#include <serde_json/detail/de_detail.hpp>

#include <cstdint>

#include <stack>

#include <glaze/glaze.hpp>
#include <glaze/base64/base64.hpp>

////////////////////////////////////////////////////////////////////////////////
// Serde JSON
////////////////////////////////////////////////////////////////////////////////
namespace serde_json {

class JsonDeserializer final : public serde::Deserializer {
   public:
    JsonDeserializer(std::string json_str) : json_str_(std::move(json_str)) {}

    std::expected<void, serde::Error> parse()
    {
        auto err_ctx = glz::read_json(parsed_json_, json_str_);
        if (err_ctx) {
            return std::unexpected(serde::Error{ .kind = serde::Error::Kind::Invalid,
                                                 .text = glz::format_error(err_ctx) });
        }
        elems_.push(parsed_json_);
        return {};
    }

    // Trivial types /////////////////////////////////////////////////////////////
    void deserialize_bool(bool& val) override { deserialize_scalar(val); }
    void deserialize_i8(int8_t& val) override { deserialize_scalar(val); }
    void deserialize_u8(uint8_t& val) override { deserialize_scalar(val); }
    void deserialize_i16(int16_t& val) override { deserialize_scalar(val); }
    void deserialize_u16(uint16_t& val) override { deserialize_scalar(val); }
    void deserialize_i32(int32_t& val) override { deserialize_scalar(val); }
    void deserialize_u32(uint32_t& val) override { deserialize_scalar(val); }
    void deserialize_i64(int64_t& val) override { deserialize_scalar(val); }
    void deserialize_u64(uint64_t& val) override { deserialize_scalar(val); }
    void deserialize_float(float& val) override { deserialize_scalar(val); }
    void deserialize_double(double& val) override { deserialize_scalar(val); }
    void deserialize_uchar(unsigned char& val) override { deserialize_scalar(val); }
    void deserialize_char(char& val) override { deserialize_cstr(std::addressof(val), 1); }

    void deserialize_length(std::size_t& len) override {}
    void deserialize_cstr(char* val, size_t len) override
    {
        // deserialize_scalar();
    }
    void deserialize_bytes(void* val, size_t len) override {}

    template<typename T>
    void deserialize_scalar(T&& val)
    {
        if (err_.has_value()) {
            return;
        }

        auto json_ref = elems_.top();
        elems_.pop();

        using type = std::remove_cvref_t<T>;
        using json_type =
            std::conditional_t<std::is_same_v<std::string, type>, std::string,
                               std::conditional_t<std::is_same_v<bool, type>, bool, double>>;

        auto& json = json_ref.get();
        if (json.holds<json_type>()) {
            auto json_val = json.get<json_type>();
            val = static_cast<std::decay_t<T>>(json_val);
        }
        else {
            err_.emplace(serde::Error{ .kind = serde::Error::Kind::Invalid,
                                       .text = "Expected bool value which not found" });
            return;
        }
    }

    // Optional //////////////////////////////////////////////////////////////////
    void deserialize_is_some(bool& val) override
    {
        auto json_ref = elems_.top();
        val = !json_ref.get().is_null();
    }

    void deserialize_none() override { elems_.pop(); }

    // Seq //////////////////////////////////////////////////////////////////////
    void deserialize_seq_begin() override {}

    void deserialize_seq_end() override {}

    void deserialize_seq_size(std::size_t& val) override {}

    // Map /////////////////////////////////////////////////////////////////////
    void deserialize_map_begin() override {}

    void deserialize_map_end() override {}

    void deserialize_map_size(std::size_t& val) override {}

    void deserialize_map_key_begin() override {}

    void deserialize_map_key_end() override {}

    void deserialize_map_key_find(const char* key) override {}

    void deserialize_map_value_begin() override {}

    void deserialize_map_value_end() override {}

    // Struct ////////////////////////////////////////////////////////////////////
    void deserialize_struct_begin() override { deserialize_map_begin(); }

    void deserialize_struct_end() override { deserialize_map_end(); }

    void deserialize_struct_field_begin(const char* name) override
    {
        deserialize_map_key_find(name);
        deserialize_map_value_begin();
    }

    void deserialize_struct_field_end() override { deserialize_map_value_end(); }

   private:
    bool await_key;
    std::optional<serde::Error> err_;
    std::string json_str_;
    std::stack<std::reference_wrapper<glz::json_t>> elems_;
    glz::json_t parsed_json_;
};

namespace detail {

auto DeserializerNew(std::string&& str) -> std::unique_ptr<serde::Deserializer>
{
    return std::make_unique<JsonDeserializer>(std::move(str));
}

auto DeserializerParse(serde::Deserializer* de) -> std::expected<void, serde::Error>
{
    auto* jsonde = static_cast<JsonDeserializer*>(de);
    return jsonde->parse();
}
}  // namespace detail

}  // namespace serde_json