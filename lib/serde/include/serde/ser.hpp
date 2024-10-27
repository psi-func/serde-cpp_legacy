#pragma once

#include <expected>
#include <concepts>

#include "ser/builtin.hpp"
#include "ser/serializer.hpp"

namespace serde::ser {

namespace serialize_impl {

// non-ADL lookup block (poison pill)
void serialize();  // undef

template<typename T, typename S>
inline constexpr bool IsSerializable = requires(T&& t, S&& serializer) {
    {
        serialize(std::forward<T>(t), std::forward<S>(serializer))
    } -> std::same_as<std::expected<typename S::Ok, typename S::Error>>;
};

struct fn {
    template<typename T, typename S>
    constexpr auto operator()(T&& obj, S&& serializer) const
    // requires Serializer<S>
    {
        if constexpr (IsSerializable<T, S>) {
            // ADL
            return serialize(std::forward<T>(obj), std::forward<S>(serializer));
        }
        else {
            // default impl
            return serde::ser::builtin::serialize(std::forward<T>(obj),
                                                  std::forward<S>(serializer));
        }
    }
};

}  // namespace serialize_impl

inline constexpr serialize_impl::fn serialize;

struct SerializerDef {

    template<Serializer Self, typename T>
    auto serialize(this Self&& self,
                   const T& v) -> std::expected<typename Self::Ok, typename Self::Error>

    {
        return serde::ser::serialize(v);
    }

    template<Serializer Self, typename K>
    auto serialize_map_key(this Self&& self,
                           const K& key) -> std::expected<typename Self::Ok, typename Self::Error>
    {
        auto res = self.serialize_map_key_begin();
        if (!res) {
            return res;
        }
        res = serde::ser::serialize(key);
        if (!res) {
            return res;
        }
        return self.serialize_map_key_end();
    }

    template<Serializer Self, typename V>
    auto serialize_map_value(this Self&& self, const V& value)
        -> std::expected<typename Self::Ok, typename Self::Error>
    {
        auto res = self.serialize_map_value_begin();
        if (!res) {
            return res;
        }
        res = serde::ser::serialize(value);
        if (!res) {
            return res;
        }
        return self.serialize_map_value_end();
    }

    template<typename Self, typename K, typename V>
    auto serialize_map_entry(this Self&& self, const K& key, const V& value)
        -> std::expected<typename Self::Ok, typename Self::Error>
    {
        auto res = self.serialize_map_key(key);
        if (!res) {
            return res;
        }
        return self.serialize_map_value(value);
    }

    template<Serializer Self, typename V>
    auto serialize_struct_field(this Self&& self, const char* name, const V& value)
        -> std::expected<typename Self::Ok, typename Self::Error>
    {
        auto res = self.serialize_struct_field_begin(name);
        if (!res) {
            return res;
        }
        res = serde::ser::serialize(value);
        if (!res) {
            return res;
        }
        return self.serialize_struct_field_end();
    }
};

}  // namespace serde::ser
