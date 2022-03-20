// Created by Xilong Yang on 2022-03-16.
//

#include "little_crypt.h"

#include <gtest/gtest.h>

#include <string>

TEST(Code, CodeConstruct) {
  little_crypt::Code code;
  EXPECT_EQ("", code.value());

  little_crypt::Code code1("Hello");
  EXPECT_EQ("Hello", code1.value());

  little_crypt::Code code2({"", 0});
  EXPECT_EQ("", code2.value());

  little_crypt::Code code3({"World", 5});
  EXPECT_EQ("World", code3.value());
}

TEST(Code, CopyAndMove) {
  little_crypt::Code code("Test");

  auto code1 = code;
  EXPECT_EQ(code.value(), code1.value());

  auto code2(code1);
  EXPECT_EQ(code1.value(), code2.value());

  auto code3(std::move(code));
  EXPECT_EQ(code2.value(), code3.value());

  auto code4 = std::move(code3);
  EXPECT_EQ(code2.value(), code4.value());
}

TEST(Code, HexValue) {
  little_crypt::Code code("");
  EXPECT_EQ(code.HexValue(), "");

  little_crypt::Code code1(std::string(1, 0x12));
  EXPECT_EQ(code1.HexValue(), "12");

  little_crypt::Code code2(std::string(4, 0x12));
  EXPECT_EQ(code2.HexValue(), "12121212");

  little_crypt::Code code3(std::string(2, 0x12));
  code3 += little_crypt::Code(std::string(2, 0x00));
  code3 += little_crypt::Code(std::string(2, 0xf0));
  code3 += little_crypt::Code(std::string(2, 0xff));
  EXPECT_EQ(code3.HexValue(), "12120000f0f0ffff");
}

TEST(Code, Operator) {
  little_crypt::Code code1("Hello");
  little_crypt::Code code2("World");

  EXPECT_EQ(code1, code1);
  EXPECT_EQ(code2, code2);

  EXPECT_EQ(code1 + code2, little_crypt::Code(code1.value() + code2.value()));
  EXPECT_EQ((code1 + code2).value(), "HelloWorld");

  code1 += code2;
  EXPECT_EQ(code1.value(), "HelloWorld");
}

TEST(Code, BaseToCode) {
  uint8_t  c = 0x00;
  uint16_t s = 0x0001;
  uint32_t i = 0x00010203;
  uint64_t l = 0x0001020304050607;

  if (little_crypt::SystemIsBigEndian()) {
    EXPECT_EQ(little_crypt::Code::BaseToCode(c).HexValue(), "00");
    EXPECT_EQ(little_crypt::Code::BaseToCode(s).HexValue(), "0001");
    EXPECT_EQ(little_crypt::Code::BaseToCode(i).HexValue(), "00010213");
    EXPECT_EQ(little_crypt::Code::BaseToCode(l).HexValue(), "0001020304050607");
  } else {
    EXPECT_EQ(little_crypt::Code::BaseToCode(c).HexValue(), "00");
    EXPECT_EQ(little_crypt::Code::BaseToCode(s).HexValue(), "0100");
    EXPECT_EQ(little_crypt::Code::BaseToCode(i).HexValue(), "03020100");
    EXPECT_EQ(little_crypt::Code::BaseToCode(l).HexValue(), "0706050403020100");
  }
}

class CodableTest : public little_crypt::CodableInterface {
 public:
  explicit CodableTest(char c = 0, int i = 0, double d = 0, const std::string& s = "")
      : c_(c), i_(i), d_(d), s_(s) {}

  little_crypt::Code Encode() override {
    little_crypt::Code result;
    result += little_crypt::Code::BaseToCode(c_);
    result += little_crypt::Code::BaseToCode(i_);
    result += little_crypt::Code::BaseToCode(d_);
    result += little_crypt::Code(s_);
    return result;
  }

  void Decode(const little_crypt::Code& code) override {
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
  std::string s_;
};

TEST(CodableInterface, CodableTest) {
  CodableTest test;
  test.c_ = 'c';
  test.i_ = 4;
  test.d_ = 2.333;
  test.s_ = "Hello";
  auto code = test.Encode();
  CodableTest test1;
  test1.Decode(code);
  EXPECT_EQ(test, test1);

  test = CodableTest();
  code = test.Encode();
  test1.Decode(code);
  EXPECT_EQ(test, test1);
}