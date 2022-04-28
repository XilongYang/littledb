// Created by Xilong Yang on 2022-03-28.
//

#include "aes128.cc"

#include <gtest/gtest.h>

using namespace littledb;

TEST(GF, GfMultiple) {
    int a = GfPlus(
      GfPlus(
          GfPlus(GfMultiple(2, 0xc9), GfMultiple(3, 0x7a))
              , GfMultiple(1, 0x63))
          , GfMultiple(1, 0xb0));
    EXPECT_EQ(a, 0xd4);

    EXPECT_EQ(GfMultiple(static_cast<Byte>(0x1234), static_cast<Byte>(0x4321))
                  , GfMultiple(static_cast<Byte>(0x4321),static_cast<Byte>(0x1234)));
}

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

  state1 ^= state2;
  EXPECT_EQ(State(code1), state1);
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

  state.SubBytes(littledb::State::INVERT);
  EXPECT_EQ(state.Encode().value(), origin);
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

  state.ShiftRows(littledb::State::INVERT);
  EXPECT_EQ(state.Encode().value(), origin);
}

TEST(State, MixColumns) {
  ByteString origin{0xd4, 0xbf, 0x5d, 0x30
                  , 0xe0, 0xb4, 0x52, 0xae
                  , 0xb8, 0x41, 0x11, 0xf1
                  , 0x1e, 0x27, 0x98, 0xe5};

  ByteString except{0x04, 0x66, 0x81, 0xe5
                  , 0xe0, 0xcb, 0x19, 0x9a
                  , 0x48, 0xf8, 0xd3, 0x7a
                  , 0x28, 0x06, 0x26, 0x4c};

  State state((Code(origin)));
  state.MixColumns();
  EXPECT_EQ(state.Encode().value(), except);

  state.MixColumns(littledb::State::INVERT);
  EXPECT_EQ(state.Encode().value(), origin);
}

TEST(State, RoundKey) {
  ByteString origin{0x2b, 0x28, 0xab, 0x09
                  , 0x7e, 0xae, 0xf7, 0xcf
                  , 0x15, 0xd2, 0x15, 0x4f
                  , 0x16, 0xa6, 0x88, 0x3c};

  State key((Code(origin)));

  auto round1_key = key.RoundKey();

  ByteString round1{0xa0, 0x88, 0x23, 0x2a
                  , 0xfa, 0x54, 0xa3, 0x6c
                  , 0xfe, 0x2c, 0x39, 0x76
                  , 0x17, 0xb1, 0x39, 0x05};

  EXPECT_EQ(round1_key.Encode().value(), round1);

  ByteString round3{0x3d, 0x47, 0x1e, 0x6d
                  , 0x80, 0x16, 0x23, 0x7a
                  , 0x47, 0xfe, 0x7e, 0x88
                  , 0x7d, 0x3e, 0x44, 0x3b};

  auto round3_key = round1_key.RoundKey().RoundKey();
  EXPECT_EQ(round3_key.Encode().value(), round3);

  ByteString round10{0xd0, 0xc9, 0xe1, 0xb6
                   , 0x14, 0xee, 0x3f, 0x63
                   , 0xf9, 0x25, 0x0c, 0x0c
                   , 0xa8, 0x89, 0xc8, 0xa6};

  auto round10_key = round3_key;
  for (int i = 0; i < 7; ++i) {
    round10_key = round10_key.RoundKey();
  }
  EXPECT_EQ(round10_key.Encode().value(), round10);

  auto rounds = GenerateRoundKeys<10>(key);
  EXPECT_EQ(rounds[0].Encode().value(), round1);
  EXPECT_EQ(rounds[2].Encode().value(), round3);
  EXPECT_EQ(rounds[9].Encode().value(), round10);
}

TEST(Aes128, EncryptAndDecrypt) {
  Code plaintext(ToByteString("HelloAes128"));
  Code key(ToByteString("yxl123456"));

  Code ciphertext = Aes128Encrypt(plaintext, key);

  auto decrypt_bytes = Aes128Decrypt(ciphertext, key).value();
  while (decrypt_bytes.back() == 0) {
    decrypt_bytes.pop_back();
  }

  Code decrypt_text(decrypt_bytes);

  EXPECT_EQ(plaintext.HexValue(), decrypt_text.HexValue());
}