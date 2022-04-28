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

//TEST(RSA, RSA1024) {
//  auto n_code = Code::FromHex(ToByteString("7db9fa70cd"));
//  auto pub_e_code = Code::FromHex(ToByteString("010001"));
//  auto pri_e_code = Code::FromHex(ToByteString("79629895aa"));
//
//  RsaInt x("709394");
//
//  auto cipher = RSA1024(x.Encode(), pub_e_code, n_code);
//  auto plain = RSA1024(cipher, pri_e_code, n_code);
//
//  RsaInt result;
//  result.Decode(plain);
//  EXPECT_EQ(result.to_string(), x.to_string());
//}

