// Created by Xilong Yang on 2022-03-19.
//

#include "little_crypt.h"

#include <gtest/gtest.h>

TEST(SwitchEndian, SwitchEndian) {
  constexpr uint16_t test = 0x1234;
  EXPECT_EQ(little_crypt::SwitchEndian(test), 0x3412);
}

TEST(SwitchEndian, BigEndian) {
  constexpr uint16_t test = 0x1234;
  if (little_crypt::SystemIsBigEndian()) {
    EXPECT_EQ(little_crypt::BigEndian(test), test);
  } else {
    EXPECT_EQ(little_crypt::BigEndian(test), little_crypt::SwitchEndian(test));
  }
}

TEST(SwitchEndian, SmallEndian) {
  constexpr uint16_t test = 0x1234;
  if (little_crypt::SystemIsBigEndian()) {
    EXPECT_EQ(little_crypt::SmallEndian(test), little_crypt::SwitchEndian(test));
  } else {
    EXPECT_EQ(little_crypt::SmallEndian(test), test);
  }
}

TEST(SwitchEndian, LocalEndian) {
  constexpr uint16_t test = 0x1234;
  if (little_crypt::SystemIsBigEndian()) {
    EXPECT_EQ(little_crypt::LocalEndian(test), test);
  } else {
    EXPECT_EQ(little_crypt::LocalEndian(test), little_crypt::SwitchEndian(test));
  }
}
