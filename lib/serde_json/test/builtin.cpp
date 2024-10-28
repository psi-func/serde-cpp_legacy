#include <gtest/gtest.h>

#include <serde/serde.hpp>
#include <serde_json/serde_json.hpp>

TEST(Basic, Bool_True)
{
    bool val = true;
    auto str = serde_json::to_string(val).value();
    EXPECT_STREQ(str.c_str(), "true");
    // auto de_val = serde_json::from_str<bool>(std::move(str)).value();
    // EXPECT_EQ(de_val, val);
}

TEST(Builtin, Bool_False)
{
    bool val = false;
    auto str = serde_json::to_string(val).value();
    EXPECT_STREQ(str.c_str(), "false");
    // auto de_val = serde_json::from_str<bool>(std::move(str)).value();
    // EXPECT_EQ(de_val, val);
}

TEST(Builtin, Int_Positive)
{
    int val = 42631;
    auto str = serde_json::to_string(val).value();
    EXPECT_STREQ(str.c_str(), "42631");
    // auto de_val = serde_json::from_str<int>(std::move(str)).value();
    // EXPECT_EQ(de_val, val);
}

TEST(Builtin, Int_Negative)
{
    int val = -42631;
    auto str = serde_json::to_string(val).value();
    EXPECT_STREQ(str.c_str(), "-42631");
    // auto de_val = serde_json::from_str<int>(std::move(str)).value();
    // EXPECT_EQ(de_val, val);
}

TEST(Builtin, ShortInt_Positive)
{
    short int val = 16535;
    auto str = serde_json::to_string(val).value();
    EXPECT_STREQ(str.c_str(), "16535");
    // auto de_val = serde_json::from_str<short int>(std::move(str)).value();
    // EXPECT_EQ(de_val, val);
}

TEST(Builtin, ShortInt_Negative)
{
    short int val = -16535;
    auto str = serde_json::to_string(val).value();
    EXPECT_STREQ(str.c_str(), "-16535");
    // auto de_val = serde_json::from_str<short int>(std::move(str)).value();
    // EXPECT_EQ(de_val, val);
}

TEST(Builtin, LongInt_Positive)
{
    long int val = 98694223l;
    auto str = serde_json::to_string(val).value();
    EXPECT_STREQ(str.c_str(), "98694223");
    // auto de_val = serde_json::from_str<long int>(std::move(str)).value();
    // EXPECT_EQ(de_val, val);
}

TEST(Builtin, LongInt_Negative)
{
    long int val = -98694223l;
    auto str = serde_json::to_string(val).value();
    EXPECT_STREQ(str.c_str(), "-98694223");
    // auto de_val = serde_json::from_str<long int>(std::move(str)).value();
    // EXPECT_EQ(de_val, val);
}

TEST(Builtin, LongLongInt_Positive)
{
    long long int val = 98694223111ll;
    auto str = serde_json::to_string(val).value();
    EXPECT_STREQ(str.c_str(), "98694223111");
    // auto de_val = serde_json::from_str<long long int>(std::move(str)).value();
    // EXPECT_EQ(de_val, val);
}

TEST(Builtin, LongLongInt_Negative)
{
    long long int val = -98694223111ll;
    auto str = serde_json::to_string(val).value();
    EXPECT_STREQ(str.c_str(), "-98694223111");
    // auto de_val = serde_json::from_str<long long int>(std::move(str)).value();
    // EXPECT_EQ(de_val, val);
}

TEST(Builtin, UnsignedInt)
{
    unsigned int val = 42631u;
    auto str = serde_json::to_string(val).value();
    EXPECT_STREQ(str.c_str(), "42631");
    // auto de_val = serde_json::from_str<unsigned int>(std::move(str)).value();
    // EXPECT_EQ(de_val, val);
}

TEST(Builtin, UnsignedShortInt)
{
    unsigned short int val = 16535u;
    auto str = serde_json::to_string(val).value();
    EXPECT_STREQ(str.c_str(), "16535");
    // auto de_val = serde_json::from_str<unsigned short int>(std::move(str)).value();
    // EXPECT_EQ(de_val, val);
}

TEST(Builtin, UnsignedLongInt)
{
    unsigned long int val = 98694223lu;
    auto str = serde_json::to_string(val).value();
    EXPECT_STREQ(str.c_str(), "98694223");
    // auto de_val = serde_json::from_str<unsigned long int>(std::move(str)).value();
    // EXPECT_EQ(de_val, val);
}

TEST(Builtin, UnsignedLongLongInt)
{
    unsigned long long int val = 98694223111llu;
    auto str = serde_json::to_string(val).value();
    EXPECT_STREQ(str.c_str(), "98694223111");
    // auto de_val = serde_json::from_str<unsigned long long int>(std::move(str)).value();
    // EXPECT_EQ(de_val, val);
}

TEST(Builtin, Float)
{
    float val = 3.14159f;
    auto str = serde_json::to_string(val).value();
    EXPECT_STREQ(str.c_str(), "3.141590118408203");
    // auto de_val = serde_json::from_str<float>(std::move(str)).value();
    // EXPECT_FLOAT_EQ(de_val, val);
}

TEST(Builtin, Double)
{
    double val = 3.14159;
    auto str = serde_json::to_string(val).value();
    EXPECT_STREQ(str.c_str(), "3.14159");
    // auto de_val = serde_json::from_str<double>(std::move(str)).value();
    // EXPECT_DOUBLE_EQ(de_val, val);
}

TEST(Builtin, Char)
{
    char val = 'A';
    auto str = serde_json::to_string(val).value();
    EXPECT_STREQ(str.c_str(), "\"A\"");
    // auto de_val = serde_json::from_str<char>(std::move(str)).value();
    // EXPECT_EQ(de_val, val);
}

TEST(Builtin, SignedChar)
{
    signed char val = 'B';
    auto str = serde_json::to_string(val).value();
    EXPECT_STREQ(str.c_str(), "\"B\"");
    // auto de_val = serde_json::from_str<char>(std::move(str)).value();
    // EXPECT_EQ(de_val, val);
}

TEST(Builtin, UnsignedChar)
{
    unsigned char val = 250;
    auto str = serde_json::to_string(val).value();
    EXPECT_STREQ(str.c_str(), "250");
    // auto de_val = serde_json::from_str<unsigned char>(std::move(str)).value();
    // EXPECT_EQ(de_val, val);
}

TEST(Builtin, ConstCharPtr)
{
    const char* val = "FooBar";
    auto str = serde_json::to_string(val).value();
    EXPECT_STREQ(str.c_str(), "\"FooBar\"");
    // EXPECT_THROW(std::ignore = serde_json::from_str<const char*>(std::move(str)),
    // std::logic_error);
}

TEST(Builtin, CharPtr)
{
    char cstr[10] = "Wiggle";
    char* val = cstr;
    auto str = serde_json::to_string(val).value();
    EXPECT_STREQ(str.c_str(), "\"Wiggle\"");
    // EXPECT_THROW(std::ignore = serde_json::from_str<char*>(std::move(str)), std::logic_error);
}

struct Struct {
    char val[10] = "Wiggle";

    template<typename S>
    friend auto serialize(const Struct& self, S&& ser)
    {
        return serde::ser::serialize(self.val, ser);
    }
    // void deserialize(serde::Deserializer& de) { de.deserialize(val); }
};

TEST(Builtin, LiteralCharArray)
{
    Struct val;
    auto str = serde_json::to_string(val).value();
    EXPECT_STREQ(str.c_str(), "\"Wiggle\"");
    // auto de_val = serde_json::from_str<Struct>(std::move(str)).value();
    // EXPECT_STREQ(de_val.val, "Wiggle");
}

struct StructBytes {
    uint8_t val[10] = { 0xde, 0xad, 0xbe, 0xef, 0x00, 0x22, 0x33, 0x44, 0x56, 0x98 };

    template<typename S>
    friend auto serialize(const StructBytes& self, S&& ser)
    {
        return ser.serialize_bytes(self.val, sizeof(self.val));
    }
    // void deserialize(serde::Deserializer& de) { de.deserialize_bytes(val, sizeof(val)); }
};

TEST(Builtin, Bytes)  // base64 encoded
{
    StructBytes val;
    auto str = serde_json::to_string(val).value();
    EXPECT_STREQ(str.c_str(), "\"3q2+7wAiM0RWmA==\"");
    // auto de_val = serde_json::from_str<Struct>(std::move(str)).value();
    // EXPECT_TRUE(0 == memcmp(val.val, de_val.val, sizeof(Struct::val)));
}
