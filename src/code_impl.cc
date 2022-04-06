// Created by Xilong Yang on 2022-03-11.
//

#include "little_crypt.h"

namespace little_crypt {
ByteString ToByteString(const std::string& str) {
  ByteString result(str.size(), 0);
  for (int i = 0; i < str.size(); ++i) {
    result[i] = str[i];
  }
  return result;
}

ByteString ToByteString(const char* c_ptr, size_t size) {
  ByteString result(size, 0);
  for (int i = 0; i < size; ++i) {
    result[i] = c_ptr[i];
  }
  return result;
}

Code::Code(const ByteString& value) {
  value_ = value;
}

const ByteString & Code::value() const { return value_; }

ByteString Code::HexValue() const {
  ByteString hex_value;
  for (const auto &c : value_) {
    const Byte kHexMap[16]{'0', '1', '2', '3'
                           , '4', '5', '6', '7'
                           , '8', '9', 'a', 'b'
                           , 'c', 'd', 'e', 'f'};
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

}  // namespace little_crypt
