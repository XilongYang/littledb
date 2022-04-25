// Created by Xilong Yang on 2022-04-25.
//

#include <gtest/gtest.h>

#include "rsa1024.cc"

using namespace littledb;

TEST(RSA, RsaInt) {
  auto code = Code::FromHex(ToByteString("22"));
  RsaInt x;
  x.Decode(code);
  EXPECT_EQ(x.to_string(), "34");
  EXPECT_EQ(x.Encode().HexValue(), code.HexValue());
}

TEST(RSA, RSA1024) {
  auto public_e= Code::FromHex(ToByteString("07"));
  auto private_e = Code::FromHex(ToByteString("03"));
  auto n = Code::FromHex(ToByteString("21"));

  RsaInt x("23");

  auto cipher = RSA1024(x.Encode(), public_e, n);
  auto plain = RSA1024(cipher, private_e, n);

  RsaInt result;
  result.Decode(plain);
  EXPECT_EQ(result.to_string(), x.to_string());

  RsaInt y("59");
  EXPECT_THROW(RSA1024(y.Encode(), public_e, n), std::logic_error);
}

