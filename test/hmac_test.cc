// Created by Xilong Yang on 2022-03-20.
//


#include "little_crypt.h"

#include <gtest/gtest.h>

TEST(Hmac, HmacSha256) {
  little_crypt::Code msg("HelloWorld");
  little_crypt::Code key("123");
  auto turn = little_crypt::Hmac(msg, key);
  EXPECT_EQ(turn.HexValue(), "ee43ed58c6e0fd87e4e80630de0d906ba194dc176f6f3ae9bf6d7008c48858a2");

  little_crypt::Code msg1(turn.HexValue());
  auto turn1 = little_crypt::Hmac(msg1, key);
  EXPECT_EQ(turn1.HexValue(), "44339c630fb6c518d1e3d1b17284852e791cb3a29c929068fc31f48191f067e3");
}

