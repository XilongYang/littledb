// Created by Xilong Yang on 2022-04-07.
//

#include "littledb.h"

#include <iterator>

namespace littledb {
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
}

std::ostream& operator<<(std::ostream& os
                         , const littledb::ByteString& byte_str) {
  for (auto b : byte_str) {
    os << b;
  }
  return os;
}

std::istream& operator>>(std::istream& is
                         , littledb::ByteString& byte_str) {
  std::istream_iterator<littledb::Byte> iter(is);
  std::istream_iterator<littledb::Byte> eof;
  while (iter != eof){
    byte_str.push_back(*iter++);
  }
  return is;
}
