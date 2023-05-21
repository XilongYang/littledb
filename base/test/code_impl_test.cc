// Created by Xilong Yang on 2022-03-16.
//

#include "code.cc"

#include <gtest/gtest.h>

using namespace littledb;

TEST(Code, CodeConstruct) {
  Code code;
  EXPECT_EQ(code.value(), ToByteString(""));

  Code code1(ToByteString("Hello"));
  EXPECT_EQ(code1.value(), ToByteString("Hello"));

  Code code2((ByteString()));
  EXPECT_EQ(code2.value(), ByteString());
}

TEST(Code, CopyAndMove) {
  Code code(littledb::ToByteString("Test"));

  auto code1 = code;
  EXPECT_EQ(code.value(), code1.value());

  auto code2(code1);
  EXPECT_EQ(code1.value(), code2.value());

  auto code3(std::move(code));
  EXPECT_EQ(code2.value(), code3.value());

  auto code4 = std::move(code3);
  EXPECT_EQ(code2.value(), code4.value());
}

TEST(Code, HexToCode) {
  Code code(ToByteString("Hello"));
  EXPECT_EQ(code.HexValue(), Code::FromHex(code.HexValue()).HexValue());
}

TEST(Code, HexValue) {
  Code code;
  EXPECT_EQ(code.HexValue(), ToByteString(""));

  Code code1(ByteString(1, 0x12));
  EXPECT_EQ(code1.HexValue(), ToByteString("12"));

  Code code2(ByteString (4, 0x12));
  EXPECT_EQ(code2.HexValue(), ToByteString("12121212"));

  Code code3(ByteString (2, 0x12));
  code3 += Code(ByteString (2, 0x00));
  code3 += Code(ByteString (2, 0xf0));
  code3 += Code(ByteString (2, 0xff));
  EXPECT_EQ(code3.HexValue(), ToByteString("12120000f0f0ffff"));
}

TEST(Code, Operator) {
  Code code1(ToByteString("Hello"));
  Code code2(ToByteString("World"));

  EXPECT_EQ(code1, code1);
  EXPECT_EQ(code2, code2);

  EXPECT_EQ(code1 + code2, Code(code1.value() + code2.value()));
  EXPECT_EQ((code1 + code2).value(), ToByteString("HelloWorld"));

  code1 += code2;
  EXPECT_EQ(code1.value(), ToByteString("HelloWorld"));
}

TEST(Code, BaseToCode) {
  uint8_t  c = 0x00;
  uint16_t s = 0x0001;
  uint32_t i = 0x00010203;
  uint64_t l = 0x0001020304050607;

  if (SystemIsBigEndian()) {
    EXPECT_EQ(Code::BaseToCode(c).HexValue()
                  , ToByteString("00"));
    EXPECT_EQ(Code::BaseToCode(s).HexValue()
                  , ToByteString("0001"));
    EXPECT_EQ(Code::BaseToCode(i).HexValue()
                  , ToByteString("00010213"));
    EXPECT_EQ(Code::BaseToCode(l).HexValue()
                  , ToByteString("0001020304050607"));
  } else {
    EXPECT_EQ(Code::BaseToCode(c).HexValue()
                  , ToByteString("00"));
    EXPECT_EQ(Code::BaseToCode(s).HexValue()
                  , ToByteString("0100"));
    EXPECT_EQ(Code::BaseToCode(i).HexValue()
                  , ToByteString("03020100"));
    EXPECT_EQ(Code::BaseToCode(l).HexValue()
                  , ToByteString("0706050403020100"));
  }
}

class CodableTest : public CodableInterface {
 public:
  explicit CodableTest(char c = 0, int i = 0, double d = 0
                       , const ByteString &s = ByteString())
      : c_(c), i_(i), d_(d), s_(s) {}

  Code Encode() override {
    Code result;
    result += Code::BaseToCode(c_);
    result += Code::BaseToCode(i_);
    result += Code::BaseToCode(d_);
    result += Code(s_);
    return result;
  }

  void Decode(const Code& code) override {
    size_t base_size = sizeof(c_) + sizeof(i_) + sizeof(d_);
    if (code.value().size() < base_size)
      return;
    c_ = code.value()[0];
    i_ = *reinterpret_cast<const int*>(&code.value()[sizeof(c_)]);
    d_ = *reinterpret_cast<const double*>(&code.value()[sizeof(c_) + sizeof(i_)]);
    s_ = code.value().substr(base_size, code.value().size() - base_size);
  }

  bool operator==(const CodableTest& test) const {
    return c_ == test.c_ && i_ == test.i_ && d_ == test.d_ && s_ == test.s_;
  }

  char c_;
  int i_;
  double d_;
  ByteString s_;
};

TEST(CodableInterface, CodableTest) {
  CodableTest test;
  test.c_ = 'c';
  test.i_ = 4;
  test.d_ = 2.333;
  test.s_ = ToByteString("Hello");
  auto code = test.Encode();
  CodableTest test1;
  test1.Decode(code);
  EXPECT_EQ(test, test1);

  test = CodableTest();
  code = test.Encode();
  test1.Decode(code);
  EXPECT_EQ(test, test1);
}