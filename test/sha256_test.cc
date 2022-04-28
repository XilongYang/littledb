// Created by Xilong Yang on 2022-03-20.
//

#include "sha256.cc"

#include <gtest/gtest.h>

using namespace littledb;
using std::array;

TEST(Sha256, Preprocess) {
  Code origin(ToByteString("HelloWorld"));
  auto turn = Preprocess(origin);
  ASSERT_EQ(turn.value().size() % 64, 0);
  EXPECT_EQ(turn.HexValue(), ToByteString("48656c6c6f576f726c64800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000050"));
}

TEST(Sha256, HashCombine) {
  array<uint32_t, 8> hash{0x1234, 0x5678, 0x9abc, 0xdef0
                          , 0x1122, 0x2233, 0x3344, 0x4455};
  auto turn = HashCombine(hash);
  ASSERT_EQ(turn.value().size(), 32);
  EXPECT_EQ(turn.HexValue(), ToByteString("000012340000567800009abc0000def000001122000022330000334400004455"));
}

TEST(Sha256, Sha256) {
  Code message(ToByteString("HelloWorld"));

  auto turn = Sha256(message);
  ASSERT_EQ(turn.value().size(), 32);
  EXPECT_EQ(turn.HexValue(), ToByteString("872e4e50ce9990d8b041330c47c9ddd11bec6b503ae9386a99da8584e9bb12c4"));

  Code message1(turn.HexValue());
  auto turn1 = Sha256(message1);
  ASSERT_EQ(turn1.value().size(), 32);
  EXPECT_EQ(turn1.HexValue(), ToByteString("2492212a9253330ada3acc8b58ba626d2e84d984da4b6f76ef8fe22e0fcb2c6b"));

  Code message2(turn1.HexValue());
  auto turn2 = Sha256(message2);
  ASSERT_EQ(turn2.value().size(), 32);
  EXPECT_EQ(turn2.HexValue(), ToByteString("a84063f724c5b60674a6466b7cec6e7fae9cd166c6f4fbfe905c7bd41c09a0a6"));

  Code message3(turn2.HexValue());
  auto turn3 = Sha256(message3);
  ASSERT_EQ(turn3.value().size(), 32);
  EXPECT_EQ(turn3.HexValue(), ToByteString("fe56f64b9130ebfc8257e52566855c9cab28536c7c2267d8b16d9976dc8ee70f"));
}
