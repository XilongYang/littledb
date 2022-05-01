// Created by Xilong Yang on 2022-04-28.
//

#include "inner_key.h"

namespace littledb{
InnerKey::InnerKey(const Code& code) : key_part1_(0), key_part2_(0) {
  InnerKey::Decode(code);
}

Code InnerKey::Encode() {
  ByteString bytes(16, 0);

  Byte* part1_ptr = reinterpret_cast<Byte*>(&key_part1_);
  Byte* part2_ptr = reinterpret_cast<Byte*>(&key_part2_);

  for (int i = 0; i < 8; ++i) {
    bytes[i] = part1_ptr[i];
    bytes[i + 8] = part2_ptr[i];
  }

  return Code(bytes);
}

void InnerKey::Decode(const Code& code) {
  if (code.value().size() != 16) {
    throw DecodeError("InnerKey code length error.");
  }
  Byte* part1_ptr = reinterpret_cast<Byte*>(&key_part1_);
  Byte* part2_ptr = reinterpret_cast<Byte*>(&key_part2_);
  for (int i = 0; i < 8; ++i) {
    part1_ptr[i] = code.value()[i];
    part2_ptr[i] = code.value()[i + 8];
  }
}

bool InnerKey::operator==(const InnerKey& other) const {
  return key_part1_ == other.key_part1_ && key_part2_ == other.key_part2_;
}

bool InnerKey::operator!=(const InnerKey& other) const {
  return !(other == *this);
}

bool InnerKey::operator<(const InnerKey& other) const {
  return key_part1_ < other.key_part1_ || key_part2_ < other.key_part2_;
}

bool InnerKey::operator>(const InnerKey& other) const {
  return key_part1_ > other.key_part1_ || key_part2_ > other.key_part2_;
}

bool InnerKey::operator<=(const InnerKey& other) const {
  return *this < other || *this == other;
}

bool InnerKey::operator>=(const InnerKey& other) const {
  return *this > other || *this == other;
}
}