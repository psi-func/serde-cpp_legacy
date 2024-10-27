#pragma once

#include <expected>

#include <serde/ser.hpp>

namespace serde::ser {

namespace detail {

template<typename T, typename S>
auto serialize_signed(T val, S&& serializer) -> std::expected<typename S::Ok, typename S::Error>
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
auto serialize_unsigned(T val, S&& serializer) -> std::expected<typename S::Ok, typename S::Error>
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
#define SER_IML_SIGNED(type)                                                                       \
    template<>                                                                                     \
    struct Serialize<type> {                                                                       \
        template<typename S>                                                                       \
        static auto serialize(type a, S&& ser) -> std::expected<typename S::Ok, typename S::Error> \
        {                                                                                          \
            return detail::serialize_signed(a, std::forward<S>(ser));                              \
        }                                                                                          \
    };

SER_IML_SIGNED(short)
SER_IML_SIGNED(int)
SER_IML_SIGNED(long)
SER_IML_SIGNED(long long)
#undef SER_IML_SIGNED

#define SER_IML_UNSIGNED(type)                                                              \
    template<>                                                                              \
    struct Serialize<type> {                                                                \
        template<typename S>                                                                \
        auto serialize(type a, S&& ser) -> std::expected<typename S::Ok, typename S::Error> \
        {                                                                                   \
            return detail::serialize_unsigned(a, std::forward<S>(ser));                     \
        }                                                                                   \
    };

SER_IML_UNSIGNED(unsigned short)
SER_IML_UNSIGNED(unsigned int)
SER_IML_UNSIGNED(unsigned long)
SER_IML_UNSIGNED(unsigned long long)
#undef SER_IML_UNSIGNED

// BOOL TYPE ////////////////////////////////////////////////////////////////////
template<typename S>
auto serialize(bool b, S&& ser) -> std::expected<typename S::Ok, typename S::Error>
{
    return ser.serialize_bool(b);
}

// FLOAT TYPES ////////////////////////////////////////////////////////////////////
template<typename S>
auto serialize(double d, S&& ser) -> std::expected<typename S::Ok, typename S::Error>
{
    return ser.serialize_double(d);
}

template<typename S>
auto serialize(float b, S&& ser) -> std::expected<typename S::Ok, typename S::Error>
{
    return ser.serialize_float(b);
}

// CHAR TYPES ////////////////////////////////////////////////////////////////////
template<typename S>
auto serialize(char b, S&& ser) -> std::expected<typename S::Ok, typename S::Error>
{
    return ser.serialize_char(b);
}

template<typename S>
auto serialize(signed char b, S&& ser) -> std::expected<typename S::Ok, typename S::Error>
{
    return ser.serialize_char(b);
}

template<typename S>
auto serialize(unsigned char b, S&& ser) -> std::expected<typename S::Ok, typename S::Error>
{
    return ser.serialize_uchar(b);
}

// STR TYPES ////////////////////////////////////////////////////////////////////
template<typename S>
auto serialize(const char* b, S&& ser) -> std::expected<typename S::Ok, typename S::Error>
{
    return ser.serialize_cstr(b);
}

template<typename S, typename T, std::size_t N>
auto serialize(T (*arr)[N], S&& ser) -> std::expected<typename S::Ok, typename S::Error>
{
    //  TODO: seq begin -> loop + serialize -> end (correct result)
    auto res_b = ser.serialize_seq_begin();
    for (auto i = 0UZ; i < N; ++i) {
        auto res = serde::ser::serialize(arr[i], ser);
    }
    return ser.serialize_seq_end();
}

}  // namespace builtin
}  // namespace serde::ser

#if 0
#include <cstdint>
#include "serialize.h"
#include "serializer.h"

namespace serde {

namespace detail {
template<typename T>
inline void serialize_signed_integer(Serializer& ser, const T& val) {
  if constexpr (sizeof(std::decay_t<T>) == 1) {
    ser.serialize_i8(val);
  }
  else if constexpr (sizeof(std::decay_t<T>) == 2) {
    ser.serialize_i16(val);
  }
  else if constexpr (sizeof(std::decay_t<T>) == 4) {
    ser.serialize_i32(val);
  }
  else if constexpr (sizeof(std::decay_t<T>) == 8) {
    ser.serialize_i64(val);
  } else {
    static_assert(sizeof(std::decay_t<T>) <= 8, "unsupported signed integer size");
  }
}

template<typename T>
inline void serialize_unsigned_integer(Serializer& ser, const T& val) {
  if constexpr (sizeof(std::decay_t<T>) == 1) {
    ser.serialize_u8(val);
  }
  else if constexpr (sizeof(std::decay_t<T>) == 2) {
    ser.serialize_u16(val);
  }
  else if constexpr (sizeof(std::decay_t<T>) == 4) {
    ser.serialize_u32(val);
  }
  else if constexpr (sizeof(std::decay_t<T>) == 8) {
    ser.serialize_u64(val);
  } else {
    static_assert(sizeof(std::decay_t<T>) <= 8, "unsupported unsigned integer size");
  }
}
} // namespace detail

template<>
inline void serialize(Serializer& ser, const bool& v)
{
  ser.serialize_bool(v);
}

template<>
inline void serialize(Serializer& ser, const int& v)
{
  detail::serialize_signed_integer(ser, v);
}

template<>
inline void serialize(Serializer& ser, const short int& v)
{
  detail::serialize_signed_integer(ser, v);
}

template<>
inline void serialize(Serializer& ser, const long int& v)
{
  detail::serialize_signed_integer(ser, v);
}

template<>
inline void serialize(Serializer& ser, const long long int& v)
{
  detail::serialize_signed_integer(ser, v);
}

template<>
inline void serialize(Serializer& ser, const unsigned int& v)
{
  detail::serialize_unsigned_integer(ser, v);
}

template<>
inline void serialize(Serializer& ser, const short unsigned int& v)
{
  detail::serialize_unsigned_integer(ser, v);
}

template<>
inline void serialize(Serializer& ser, const long unsigned int& v)
{
  detail::serialize_unsigned_integer(ser, v);
}

template<>
inline void serialize(Serializer& ser, const long long unsigned int& v)
{
  detail::serialize_unsigned_integer(ser, v);
}

template<>
inline void serialize(Serializer& ser, const float& v)
{
  ser.serialize_float(v);
}

template<>
inline void serialize(Serializer& ser, const double& v)
{
  ser.serialize_double(v);
}

template<>
inline void serialize(Serializer& ser, const char& v)
{
  ser.serialize_char(v);
}

template<>
inline void serialize(Serializer& ser, const signed char& v)
{
  ser.serialize_char(v);
}

template<>
inline void serialize(Serializer& ser, const unsigned char& v)
{
  ser.serialize_uchar(v);
}

template<>
inline void serialize(Serializer& ser, char* const& val)
{
  ser.serialize_cstr(val);
}

template<>
inline void serialize(Serializer& ser, const char* const& val)
{
  ser.serialize_cstr(val);
}

template<size_t N>
inline void serialize(Serializer& ser, const char (&val)[N])
{
  ser.serialize_cstr(val);
}

} // namespace serde

#endif
