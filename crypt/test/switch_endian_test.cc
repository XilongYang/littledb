// Created by Xilong Yang on 2022-03-19.
//

#include "littledb.h"

#include <gtest/gtest.h>

using namespace littledb;

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

TEST(SwitchEndian, LittleEndian) {
  constexpr uint16_t test = 0x1234;
  if (SystemIsBigEndian()) {
    EXPECT_EQ(LittleEndian(test), SwitchEndian(test));
  } else {
    EXPECT_EQ(LittleEndian(test), test);
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
