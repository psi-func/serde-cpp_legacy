#pragma once

#include <expected>

namespace serde::ser {

namespace detail {

template<typename T, typename S>
auto serialize_signed(T val, S&& serializer)
    -> std::expected<typename std::remove_cvref_t<S>::Ok, typename std::remove_cvref_t<S>::Error>
{
    if constexpr (sizeof(std::decay_t<T>) == 1) {
        return serializer.serialize_i8(val);
    }
    else if constexpr (sizeof(std::decay_t<T>) == 2) {
        return serializer.serialize_i16(val);
    }
    else if constexpr (sizeof(std::decay_t<T>) == 4) {
        return serializer.serialize_i32(val);
    }
    else if constexpr (sizeof(std::decay_t<T>) == 8) {
        return serializer.serialize_i64(val);
    }
    else {
        static_assert(sizeof(std::decay_t<T>) <= 8, "unsupported signed integer size");
    }
}

template<typename T, typename S>
auto serialize_unsigned(T val, S&& serializer)
    -> std::expected<typename std::remove_cvref_t<S>::Ok, typename std::remove_cvref_t<S>::Error>
{
    if constexpr (sizeof(std::decay_t<T>) == 1) {
        return serializer.serialize_u8(val);
    }
    else if constexpr (sizeof(std::decay_t<T>) == 2) {
        return serializer.serialize_u16(val);
    }
    else if constexpr (sizeof(std::decay_t<T>) == 4) {
        return serializer.serialize_u32(val);
    }
    else if constexpr (sizeof(std::decay_t<T>) == 8) {
        return serializer.serialize_u64(val);
    }
    else {
        static_assert(sizeof(std::decay_t<T>) <= 8, "unsupported signed integer size");
    }
}

}  // namespace detail

namespace builtin {

// INTEGER TYPES ////////////////////////////////////////////////////////////////////
#define SER_IML_SIGNED(type)                                                                \
    template<typename S>                                                                    \
    static auto serialize(type a,                                                           \
                          S&& ser) -> std::expected<typename std::remove_cvref_t<S>::Ok,    \
                                                    typename std::remove_cvref_t<S>::Error> \
    {                                                                                       \
        return detail::serialize_signed(a, std::forward<S>(ser));                           \
    }

SER_IML_SIGNED(short)
SER_IML_SIGNED(int)
SER_IML_SIGNED(long)
SER_IML_SIGNED(long long)
#undef SER_IML_SIGNED

#define SER_IML_UNSIGNED(type)                                                               \
    template<typename S>                                                                     \
    auto serialize(type a, S&& ser) -> std::expected<typename std::remove_cvref_t<S>::Ok,    \
                                                     typename std::remove_cvref_t<S>::Error> \
    {                                                                                        \
        return detail::serialize_unsigned(a, std::forward<S>(ser));                          \
    }

SER_IML_UNSIGNED(unsigned short)
SER_IML_UNSIGNED(unsigned int)
SER_IML_UNSIGNED(unsigned long)
SER_IML_UNSIGNED(unsigned long long)
#undef SER_IML_UNSIGNED

// BOOL TYPE ////////////////////////////////////////////////////////////////////
template<typename S>
auto serialize(bool b, S&& ser)
    -> std::expected<typename std::remove_cvref_t<S>::Ok, typename std::remove_cvref_t<S>::Error>
{
    return ser.serialize_bool(b);
}

// FLOAT TYPES ////////////////////////////////////////////////////////////////////
template<typename S>
auto serialize(double d, S&& ser)
    -> std::expected<typename std::remove_cvref_t<S>::Ok, typename std::remove_cvref_t<S>::Error>
{
    return ser.serialize_double(d);
}

template<typename S>
auto serialize(float b, S&& ser)
    -> std::expected<typename std::remove_cvref_t<S>::Ok, typename std::remove_cvref_t<S>::Error>
{
    return ser.serialize_float(b);
}

// CHAR TYPES ////////////////////////////////////////////////////////////////////
template<typename S>
auto serialize(char b, S&& ser)
    -> std::expected<typename std::remove_cvref_t<S>::Ok, typename std::remove_cvref_t<S>::Error>
{
    return ser.serialize_char(b);
}

template<typename S>
auto serialize(signed char b, S&& ser)
    -> std::expected<typename std::remove_cvref_t<S>::Ok, typename std::remove_cvref_t<S>::Error>
{
    return ser.serialize_char(b);
}

template<typename S>
auto serialize(unsigned char b, S&& ser)
    -> std::expected<typename std::remove_cvref_t<S>::Ok, typename std::remove_cvref_t<S>::Error>
{
    return ser.serialize_uchar(b);
}

// STR TYPES ////////////////////////////////////////////////////////////////////
template<typename S>
auto serialize(const char* b, S&& ser)
    -> std::expected<typename std::remove_cvref_t<S>::Ok, typename std::remove_cvref_t<S>::Error>
{
    return ser.serialize_cstr(b);
}

}  // namespace builtin
}  // namespace serde::ser
