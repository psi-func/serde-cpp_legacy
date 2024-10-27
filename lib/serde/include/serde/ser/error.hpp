#include <concepts>
#include <string>

namespace serde::ser {

template<typename E>
concept Error = requires(std::string msg) {
    { E::custom(msg) } -> std::same_as<E>;
};

}  // namespace serde::ser