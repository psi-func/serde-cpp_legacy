#include <serde_json/detail/de_detail.hpp>

#include <cstdint>

#include <glaze/glaze.hpp>
#include <glaze/base64/base64.hpp>

namespace serde_json {

class JsonDeserializer final : public serde::Deserializer {
   public:
    JsonDeserializer(std::string json_str) : json_str_(std::move(json_str)) {}

    std::expected<void, serde::Error> parse() { return {}; }

    // Trivial types /////////////////////////////////////////////////////////////
    void deserialize_bool(bool& val) override {}
    void deserialize_i8(int8_t& val) override {}
    void deserialize_u8(uint8_t& val) override {}
    void deserialize_i16(int16_t& val) override {}
    void deserialize_u16(uint16_t& val) override {}
    void deserialize_i32(int32_t& val) override {}
    void deserialize_u32(uint32_t& val) override {}
    void deserialize_i64(int64_t& val) override {}
    void deserialize_u64(uint64_t& val) override {}
    void deserialize_float(float& val) override {}
    void deserialize_double(double& val) override {}
    void deserialize_char(char& val) override {}
    void deserialize_uchar(unsigned char& val) override {}
    void deserialize_cstr(char* val, size_t len) override {}
    void deserialize_bytes(void* val, size_t len) override {}

    void deserialize_length(std::size_t& len) override {}

    // Optional //////////////////////////////////////////////////////////////////
    void deserialize_is_some(bool& val) override {}

    void deserialize_none() override {}

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
    std::string json_str_;
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