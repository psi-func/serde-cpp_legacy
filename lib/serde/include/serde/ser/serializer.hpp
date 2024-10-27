#pragma once

#include <cstdint>
#include <concepts>
#include <expected>

#include "error.hpp"

namespace serde::ser {

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

        // Struct ////////////////////////////////////////////////////////////////////
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

#if 0




////////////////////////////////////////////////////////////////////////////////
/// Serializer interface
///
/// Datatypes should implement serde::serialize function and call the serializer
/// methods to serialize primitive types, sequences, maps, structs, etc.
///
/// Dataformats should implement the methods in this interface to serialize
/// datatypes to its corresponding dataformat, e.g. yaml, json, toml, etc.
/// Conventionally Serializers should provide to_string() and from_str()
/// functions for serializing any datatype to string and deserializing from
/// string to any datatype.
class Serializer {
public:
  // serde::serialize should be implemented (specialized) for user types.
  // most std types have builtin implementation from the library
  template<typename T>
  inline void serialize(const T& v) {
    if constexpr (traits::HasMemberSerialize<T>::value) v.serialize(*this);
    else if constexpr (traits::HasSerialize<T>::value) Serialize<T>::serialize(*this, v);
    else serde::serialize(*this, v);
  }

  // Scalars ///////////////////////////////////////////////////////////////////
  // note: prefer using serde::serialize() for standard types
  // int, unsigned int, short int, long int, size_t, intptr_t ...
  // as they will get resolved to corresponding fixed width integer types by
  // the compiler and serde::serialize() is implemented for those scalars.
  virtual void serialize_bool(bool) = 0;
  virtual void serialize_i8(int8_t) = 0;
  virtual void serialize_u8(uint8_t) = 0;
  virtual void serialize_i16(int16_t) = 0;
  virtual void serialize_u16(uint16_t) = 0;
  virtual void serialize_i32(int32_t) = 0;
  virtual void serialize_u32(uint32_t) = 0;
  virtual void serialize_i64(int64_t) = 0;
  virtual void serialize_u64(uint64_t) = 0;
  virtual void serialize_float(float) = 0;
  virtual void serialize_double(double) = 0;
  virtual void serialize_char(char) = 0;
  virtual void serialize_uchar(unsigned char) = 0;
  virtual void serialize_cstr(const char*) = 0;
  virtual void serialize_bytes(const void* val, size_t len) = 0;

  // Optional //////////////////////////////////////////////////////////////////
  virtual void serialize_none() = 0;

  // Sequence //////////////////////////////////////////////////////////////////
  virtual void serialize_seq_begin() = 0;
  virtual void serialize_seq_end() = 0;

  // Map ///////////////////////////////////////////////////////////////////////
  virtual void serialize_map_begin() = 0;
  virtual void serialize_map_end() = 0;
  virtual void serialize_map_key_begin() = 0;
  virtual void serialize_map_key_end() = 0;
  virtual void serialize_map_value_begin() = 0;
  virtual void serialize_map_value_end() = 0;

  template<typename K>
  inline void serialize_map_key(const K& key) {
    serialize_map_key_begin();
    serialize(key);
    serialize_map_key_end();
  }

  template<typename V>
  inline void serialize_map_value(const V& value) {
    serialize_map_value_begin();
    serialize(value);
    serialize_map_value_end();
  }

  template<typename K, typename V>
  inline void serialize_map_entry(const K& key, const V& value) {
    serialize_map_key(key);
    serialize_map_value(value);
  }

  // Struct ////////////////////////////////////////////////////////////////////
  virtual void serialize_struct_begin() = 0;
  virtual void serialize_struct_end() = 0;
  virtual void serialize_struct_field_begin(const char* name) = 0;
  virtual void serialize_struct_field_end() = 0;

  template<typename V>
  inline void serialize_struct_field(const char* name, const V& value) {
    serialize_struct_field_begin(name);
    serialize(value);
    serialize_struct_field_end();
  }

  // Flat //////////////////////////////////////////////////////////////////////
  // template<typename T> void serialize_flat(const T& v);
  // virtual void serialize_flat_begin() = 0;
  // virtual void serialize_flat_end() = 0;

  // Destructor
  virtual ~Serializer() = default;
};
#endif

}  // namespace serde::ser
