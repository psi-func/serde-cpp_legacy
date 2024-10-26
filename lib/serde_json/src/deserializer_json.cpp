#include <serde_json/detail/de_detail.hpp>

#include <glaze/glaze.hpp>

#include <glaze/base64/base64.hpp>

namespace serde_json {

class JsonDeserializer final : public serde::Deserializer {
   public:
    JsonDeserializer(std::string json_str) : json_str_(std::move(json_str)) {}

    std::expected<void, serde::Error> parse();

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