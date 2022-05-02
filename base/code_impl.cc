// Created by Xilong Yang on 2022-03-11.
//

#include "littledb.h"

namespace littledb {
Code::Code(const ByteString& value) {
  value_ = value;
}

const ByteString & Code::value() const { return value_; }

namespace {
const Byte kHexMap[16]{'0', '1', '2', '3'
    , '4', '5', '6', '7'
    , '8', '9', 'a', 'b'
    , 'c', 'd', 'e', 'f'};

Byte HexToNum(Byte b) {
  Byte tmp;
  if (b >= '0' && b <= '9') {
    tmp = b - '0';
  } else if (b >= 'a' && b <= 'f') {
    tmp = b - 'a' + 10;
  } else if (b >= 'A' && b <= 'F') {
    tmp = b - 'A' + 10;
  } else {
    throw DecodeError("Input not a hexadecimal number.");
  }
  return tmp;
}
}

ByteString Code::HexValue() const {
  ByteString hex_value;
  for (const auto &c : value_) {
    hex_value.push_back(kHexMap[c / 16]);
    hex_value.push_back(kHexMap[c % 16]);
  }
  return hex_value;
}

bool Code::operator==(const Code& code) const {
  return value() == code.value();
}

bool Code::operator!=(const Code& code) const {
  return value() != code.value();
}

const Code Code::operator+(const Code& code) const {
  return Code(value() + code.value());
}

Code& Code::operator+=(const Code& code) {
  *this = *this + code;
  return *this;
}

// Get the code for a hexadecimal number.
Code Code::FromHex(const ByteString& hex) {
  ByteString value;
  for (int i = 0; i < hex.length(); i += 2) {
    Byte tmp = HexToNum(hex[i]);
    tmp <<= 4;
    tmp += HexToNum(hex[i + 1]);
    value.push_back(tmp);
  }
  return Code(value);
}

}  // namespace littledb
