#pragma once

#include <expected>
#include <concepts>
#include <string>

#include "ser/builtin.h"

namespace serde::ser {

template<typename E>
concept Error = requires(std::string msg) {
    { E::custom(msg) } -> std::same_as<E>;
};

template<typename T>
concept Serializer =
    requires(T&& t, bool b, int8_t i8, int16_t i16, int32_t i32, int64_t i64, uint8_t u8,
             uint16_t u16, uint32_t u32, uint64_t u64, float f, double d, char ch,
             unsigned char uch, const char* cstr, const void* bytes, std::size_t sz) {
        typename T::Ok;
        typename T::Error;

        requires Error<typename T::Error>;

        // Scalars ///////////////////////////////////////////////////////////////////
        { t.serialize_bool(b) } -> std::same_as<std::expected<typename T::Ok, typename T::Error>>;
        { t.serialize_i8(i8) } -> std::same_as<std::expected<typename T::Ok, typename T::Error>>;
        { t.serialize_u8(u8) } -> std::same_as<std::expected<typename T::Ok, typename T::Error>>;
        { t.serialize_i16(i16) } -> std::same_as<std::expected<typename T::Ok, typename T::Error>>;
        { t.serialize_u16(u16) } -> std::same_as<std::expected<typename T::Ok, typename T::Error>>;
        { t.serialize_i32(i32) } -> std::same_as<std::expected<typename T::Ok, typename T::Error>>;
        { t.serialize_u32(u32) } -> std::same_as<std::expected<typename T::Ok, typename T::Error>>;
        { t.serialize_i64(i64) } -> std::same_as<std::expected<typename T::Ok, typename T::Error>>;
        { t.serialize_u64(u64) } -> std::same_as<std::expected<typename T::Ok, typename T::Error>>;
        { t.serialize_float(f) } -> std::same_as<std::expected<typename T::Ok, typename T::Error>>;
        { t.serialize_double(d) } -> std::same_as<std::expected<typename T::Ok, typename T::Error>>;
        { t.serialize_char(ch) } -> std::same_as<std::expected<typename T::Ok, typename T::Error>>;
        {
            t.serialize_uchar(uch)
        } -> std::same_as<std::expected<typename T::Ok, typename T::Error>>;
        {
            t.serialize_cstr(cstr)
        } -> std::same_as<std::expected<typename T::Ok, typename T::Error>>;
        {
            t.serialize_bytes(bytes, sz)
        } -> std::same_as<std::expected<typename T::Ok, typename T::Error>>;

        // Optional //////////////////////////////////////////////////////////////////
        { t.serialize_none() } -> std::same_as<std::expected<typename T::Ok, typename T::Error>>;

        // Sequence //////////////////////////////////////////////////////////////////
        {
            t.serialize_seq_begin()
        } -> std::same_as<std::expected<typename T::Ok, typename T::Error>>;
        { t.serialize_seq_end() } -> std::same_as<std::expected<typename T::Ok, typename T::Error>>;

        // Map ///////////////////////////////////////////////////////////////////////
        {
            t.serialize_map_begin()
        } -> std::same_as<std::expected<typename T::Ok, typename T::Error>>;
        { t.serialize_map_end() } -> std::same_as<std::expected<typename T::Ok, typename T::Error>>;
        {
            t.serialize_map_key_begin()
        } -> std::same_as<std::expected<typename T::Ok, typename T::Error>>;
        {
            t.serialize_map_key_end()
        } -> std::same_as<std::expected<typename T::Ok, typename T::Error>>;
        {
            t.serialize_map_value_begin()
        } -> std::same_as<std::expected<typename T::Ok, typename T::Error>>;
        {
            t.serialize_map_value_end()
        } -> std::same_as<std::expected<typename T::Ok, typename T::Error>>;
        {
            t.serialize_struct_begin()
        } -> std::same_as<std::expected<typename T::Ok, typename T::Error>>;
        {
            t.serialize_struct_end()
        } -> std::same_as<std::expected<typename T::Ok, typename T::Error>>;
        {
            t.serialize_struct_field_begin(cstr)
        } -> std::same_as<std::expected<typename T::Ok, typename T::Error>>;
        {
            t.serialize_struct_field_end()
        } -> std::same_as<std::expected<typename T::Ok, typename T::Error>>;
    };

template<typename T, typename S>
concept Serialize = requires(T&& t, S&& serializer) {
    {
        serialize(std::forward<T>(t), std::forward<S>(serializer))
    } -> std::same_as<std::expected<typename S::Ok, typename S::Error>>;  // ADL
};

namespace serialize_impl {

struct fn {
    template<typename T, typename S>
    constexpr auto operator()(T&& obj, S&& serializer) const
    {
        if constexpr (ser::Serialize<T, S>) {
            // ADL
            serialize(std::forward<T>(obj), std::forward<S>(serializer));
        }
        else {
            // default impl
            ::serde::ser::builtin::serialize(std::forward<T>(obj), std::forward<S>(serializer));
        }
    }
};

}  // namespace serialize_impl

inline constexpr serialize_impl::fn serialize;

}  // namespace serde::ser
