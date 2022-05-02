// Created by Xilong Yang on 2022-03-20.
//


#include <gtest/gtest.h>

#include "inner_key.h"

using namespace littledb;

TEST(InnerKey, Transform) {
  auto correct_key = Code(
      ByteString(
          Sha256(Code(ToByteString("Hello"))).value()
              , 0, 16));
  auto error_key = Code(ToByteString("Hello"));
  InnerKey k1;
  k1.Decode(correct_key);
  InnerKey k2(correct_key);
  EXPECT_EQ(k1, k2);
  EXPECT_EQ(k1.Encode(), correct_key);
  EXPECT_EQ(k2.Encode(), correct_key);

  EXPECT_THROW(k1.Decode(error_key), DecodeError);
  EXPECT_THROW(k2.Decode(error_key), DecodeError);
}
