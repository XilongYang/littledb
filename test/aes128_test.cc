// Created by Xilong Yang on 2022-03-28.
//

#include "aes128.cc"

#include <gtest/gtest.h>

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
  little_crypt::Code code("The length is 16");
  State state(code);
  EXPECT_EQ(code, state.Encode());

  little_crypt::Code code1("The length is not 16");
  State state1;
  EXPECT_THROW(state1.Decode(code1), little_crypt::DecodeError);
}

TEST(State, Operator) {
  little_crypt::Code code(std::string(16, 'a'));
  State state(code);

  state[2] = 'b';
  EXPECT_EQ(state.Encode(), little_crypt::Code("aabaaaaaaaaaaaaa"));

  state[3] = 'c';
  EXPECT_EQ(state.Encode(), little_crypt::Code("aabcaaaaaaaaaaaa"));

  const auto state1 = state;
  state[0] = state1[2];
  EXPECT_EQ(state.Encode(), little_crypt::Code("babcaaaaaaaaaaaa"));
}

TEST(State, Xor) {
  std::string str1(16, 'a');
  std::string str2(16, 'b');
  std::string str3(16, 0);
  for (int i = 0; i < 16; ++i) {
    str3[i] = str1[i]^str2[i];
  }

  little_crypt::Code code1(str1);
  little_crypt::Code code2(str2);
  little_crypt::Code code3(str3);

  State state1(code1);
  State state2(code2);
  State state3(code3);

  State xor_state = state1^state2;
  EXPECT_EQ(xor_state, state3);

  state1 ^= state2;
  EXPECT_EQ(state1, xor_state);
}
