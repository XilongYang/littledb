// Created by Xilong Yang on 2022-04-07.
//

#include "little_crypt.h"

#include <gtest/gtest.h>
#include <sstream>
#include <iostream>

using namespace little_crypt;

TEST(ByteString, ToByteString) {
ByteString str1{'H', 'e', 'l', 'l', 'o'};
auto str2 = ToByteString("Hello");
auto str3 = ToByteString("Hello", 5);

EXPECT_EQ(str1, str2);
EXPECT_EQ(str1, str3);
}

TEST(ByteString, IO) {
  auto str = ToByteString("Hello");
  std::ostringstream out;
  out << str;
  EXPECT_EQ(out.str(), "Hello");

  std::istringstream in("Hello");
  ByteString str1;
  in >> str1;
  EXPECT_EQ(str1, str);

  std::cout << str << std::endl;
}
