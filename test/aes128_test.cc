// Created by Xilong Yang on 2022-03-28.
//

#include "aes128.cc"

#include <gtest/gtest.h>

using namespace littledb;

TEST(State, ConstructAndEqual) {
  State state;
  State state1(state);
  EXPECT_EQ(state, state1);

  State state2(std::move(state1));
  EXPECT_EQ(state, state2);

  State state3 = state2;
  EXPECT_EQ(state, state3);

  State state4 = std::move(state3);
  EXPECT_EQ(state, state4);
}

TEST(State, EncodeAndDecode) {
  Code code(ToByteString("The length is 16"));
  State state(code);
  EXPECT_EQ(code, state.Encode());

  Code code1(ToByteString("The length is not 16"));
  State state1;
  EXPECT_THROW(state1.Decode(code1), DecodeError);
}

TEST(State, Operator) {
  Code code(ByteString(16, 'a'));
  State state(code);

  state[2][0] = 'b';
  EXPECT_EQ(state.Encode(), Code(ToByteString("aabaaaaaaaaaaaaa")));

  state[3][0] = 'c';
  EXPECT_EQ(state.Encode(), Code(ToByteString("aabcaaaaaaaaaaaa")));

  const auto state1 = state;
  state[0][0] = state1[2][0];
  EXPECT_EQ(state.Encode(), Code(ToByteString("babcaaaaaaaaaaaa")));
}

TEST(State, Xor) {
  ByteString str1(16, 'a');
  ByteString str2(16, 'b');
  ByteString str3(16, 0);
  for (int i = 0; i < 16; ++i) {
    str3[i] = str1[i]^str2[i];
  }

  Code code1(str1);
  Code code2(str2);
  Code code3(str3);

  State state1(code1);
  State state2(code2);
  State state3(code3);

  State xor_state = state1^state2;
  EXPECT_EQ(xor_state, state3);

  state1 ^= state2;
  EXPECT_EQ(state1, xor_state);
}

TEST(State, SubBytes) {
  ByteString origin{  0x19, 0x3d, 0xe3, 0xbe
                    , 0xa0, 0xf4, 0xe2, 0x2b
                    , 0x9a, 0xc6, 0x8d, 0x2a
                    , 0xe9, 0xf8, 0x48, 0x08};

  ByteString except{  0xd4, 0x27, 0x11, 0xae
                    , 0xe0, 0xbf, 0x98, 0xf1
                    , 0xb8, 0xb4, 0x5d, 0xe5
                    , 0x1e, 0x41, 0x52, 0x30};

  State state((Code(origin)));
  state.SubBytes();
  EXPECT_EQ(state.Encode().value(), except);
}

TEST(State, ShiftRows) {
  ByteString origin{1, 5, 9, 13
                  , 2, 6, 10, 14
                  , 3, 7, 11, 15
                  , 4, 8, 12, 16};

  ByteString except{1, 5, 9, 13
                  , 6, 10, 14, 2
                  , 11, 15, 3, 7
                  , 16, 4, 8, 12};

  State state((Code(origin)));
  state.ShiftRows();
  EXPECT_EQ(state.Encode().value(), except);
}