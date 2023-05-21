// Created by Xilong Yang on 2022-03-11.
//

#include "littledb.h"

namespace littledb {

/** ===== class CodeImpl ===== **/
class Code::CodeImpl {
public:
    CodeImpl();
    CodeImpl(Size size, Byte byte);
    CodeImpl(char* str, Size size);
    explicit CodeImpl(std::string const &str);
    explicit CodeImpl(std::basic_string<Byte> const &str);

    std::basic_string<Byte> GetValue();
    void SetValue(std::basic_string<Byte> value);
private:
    std::basic_string<Byte> value_;
};

Code::CodeImpl::CodeImpl() : value_() {}

Code::CodeImpl::CodeImpl(Size size, Byte byte) : value_(std::basic_string<Byte>(size, byte)) {}

Code::CodeImpl::CodeImpl(char* str, Size size) : value_(size, 0) {
    for (Size i = 0; i < size; ++i) {
        value_[i] = str[i];
    }
}

Code::CodeImpl::CodeImpl(std::string const &str) : value_(str.size(), 0) {
    for (Size i = 0; i < str.size(); ++i) {
        value_[i] = str[i];
    }
}

Code::CodeImpl::CodeImpl(std::basic_string<Byte> const &str) : value_(str.size(), 0) {
    for (Size i = 0; i < str.size(); ++i) {
        value_[i] = str[i];
    }
}

std::basic_string<Byte> Code::CodeImpl::GetValue() {return value_;}

void Code::CodeImpl::SetValue(std::basic_string<Byte> value) {value_ = value;}

/** ===== class Code ===== **/
Code::Code() : impl_(std::make_unique<CodeImpl>()) {}
Code::~Code() = default;
Code::Code(Code const &code) : impl_(std::make_unique<CodeImpl>(code.impl_->GetValue())) {}
Code::Code(Code &&code) : impl_(std::move(code.impl_)) {}
Code::Code(Size size, Byte byte) : impl_(std::make_unique<CodeImpl>(size, byte)) {}
Code::Code(char* str, Size size) : impl_(std::make_unique<CodeImpl>(str, size)) {}
Code::Code(const std::string& str) : impl_(std::make_unique<CodeImpl>(str)) {}

Size Code::ByteSum() const {
    return impl_->GetValue().size();
}

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
