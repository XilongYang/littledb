// Created by Xilong Yang on 2022-03-19.
//

#include "little_crypt.h"

#include <gtest/gtest.h>

using namespace little_crypt;

TEST(SwitchEndian, SwitchEndian) {
  constexpr uint16_t test = 0x1234;
  EXPECT_EQ(SwitchEndian(test), 0x3412);
}

TEST(SwitchEndian, BigEndian) {
  constexpr uint16_t test = 0x1234;
  if (SystemIsBigEndian()) {
    EXPECT_EQ(BigEndian(test), test);
  } else {
    EXPECT_EQ(BigEndian(test), SwitchEndian(test));
  }
}

TEST(SwitchEndian, SmallEndian) {
  constexpr uint16_t test = 0x1234;
  if (SystemIsBigEndian()) {
    EXPECT_EQ(SmallEndian(test), SwitchEndian(test));
  } else {
    EXPECT_EQ(SmallEndian(test), test);
  }
}

TEST(SwitchEndian, LocalEndian) {
  constexpr uint16_t test = 0x1234;
  if (SystemIsBigEndian()) {
    EXPECT_EQ(LocalEndian(test), test);
  } else {
    EXPECT_EQ(LocalEndian(test), SwitchEndian(test));
  }
}
