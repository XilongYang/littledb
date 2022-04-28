// Created by Xilong Yang on 2022-04-24.
//

#include "littledb.h"
#include "BigInt.h"
#include <iostream>

namespace littledb {
namespace {
std::string ToString(const ByteString& byte_str) {
  std::string str;
  for (auto c : byte_str) {
    str.push_back(c);
  }
  return str;
}

const Byte kHexMap[16]{'0', '1', '2', '3', '4', '5', '6', '7',
                       '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

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

class RsaInt : public BigInt, public CodableInterface {
 public:
  RsaInt() : BigInt() {}
  explicit RsaInt(const std::string& num) : BigInt(num) {}
  explicit RsaInt(const ByteString& num) : BigInt(ToString(num)) {}

  Code Encode() override {
    ByteString dec_value = ToByteString(to_string());
    ByteString bin_value;
    while (!dec_value.empty()) {
      ByteString new_dec_value;
      Byte remain = 0;
      for (int i = 0; i < dec_value.length(); ++i) {
        if (dec_value[i] < '0' || dec_value[i] > '9') {
          throw DecodeError("Input is not decimal number.");
        }
        int tmp = remain * 10 + dec_value[i] - '0';
        if (!new_dec_value.empty() || tmp >= 2) {
          new_dec_value.push_back(kHexMap[tmp / 2]);
        }
        remain = tmp % 2;
      }
      bin_value.push_back(remain + '0');
      dec_value = new_dec_value;
    }
    while (bin_value.length() % 8 != 0) {
      bin_value.push_back('0');
    }
    ByteString hex_value;
    for (int i = 0; i < bin_value.length(); i += 8) {
      Byte tmp = 0;
      for (int j = i + 7; j > i; --j) {
        if (bin_value[j] == '1') {
          tmp += 1;
        }
        tmp <<= 1;
      }
      if (bin_value[i] == '1') {
        tmp += 1;
      }
      hex_value.push_back(tmp);
    }
    return Code(ByteString(hex_value.rbegin(), hex_value.rend()));
  }

  void Decode(const Code& code) override {
    ByteString hex_value = code.HexValue();
    ByteString dec_value;
    while (!hex_value.empty()) {
      ByteString new_hex_value;
      Byte remain = 0;
      for (int i = 0; i < hex_value.length(); ++i) {
        int tmp = remain * 16 + HexToNum(hex_value[i]);
        if (!new_hex_value.empty() || tmp >= 10) {
          new_hex_value.push_back(kHexMap[tmp / 10]);
        }
        remain = tmp % 10;
      }
      dec_value.push_back(remain + '0');
      hex_value = new_hex_value;
    }

    auto num = ByteString(dec_value.rbegin(), dec_value.rend());
    *this = RsaInt(num);
  }
};
}

Code Rsa1024(const Code& text, const Code& exponents, const Code& modulus) {
//  RsaInt x, e, n;
//  x.Decode(text);
//  e.Decode(exponents);
//  n.Decode(modulus);

//  if (x > n) {
//    throw std::logic_error("Text is so long that RSA1024 can not deal.");
//  }
//
//  RsaInt result = x;
//  for (RsaInt i("1"); i < e; ++i) {
//    result *= x;
//    result %= n;
//    std::cout << i << "-" << e << std::endl;
//  }
//  auto result_code = result.Encode();
  auto result_code = text;
  auto padding = Code(ByteString(128 - result_code.value().length(), 0));
  return padding + result_code;
}
}

