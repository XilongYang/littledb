// Created by Xilong Yang on 2022-03-11.
//

#include "little_crypt.h"

#include <string>
#include <sstream>
#include <iomanip>

namespace little_crypt {
class Code::CodeImpl {
 public:
  CodeImpl() = default;
  ~CodeImpl() = default;
  CodeImpl(const CodeImpl&) = default;
  CodeImpl(CodeImpl&&) = default;

  explicit CodeImpl(const std::string& value) : value_(value) {}
  explicit CodeImpl(const char* c_ptr, const std::size_t& size)
      : value_(c_ptr, size) {}

  [[nodiscard]] const std::string& value() const { return value_;}

 private:
  std::string value_;
};

Code::Code() {
    impl_ = std::make_unique<Code::CodeImpl>("");
};

Code::~Code() = default;

Code::Code(const Code& code) {
  impl_ = std::make_unique<Code::CodeImpl>(code.value());
}

Code::Code(Code&& code) noexcept {
  impl_ = std::move(code.impl_);
}

Code& Code::operator=(const Code& code) {
  impl_ = std::make_unique<CodeImpl>(code.value());
  return *this;
};

Code& Code::operator=(Code&& code) noexcept {
  impl_ = std::move(code.impl_);
  return *this;
};

Code::Code(const std::string& value) {
  impl_ = std::make_unique<Code::CodeImpl>(value);
}

const std::string& Code::value() const { return impl_->value(); }

std::string Code::HexValue() const {
  std::ostringstream hex_stream;
  hex_stream << std::hex << std::setfill('0') << std::right;

  auto raw_value = impl_->value();
  for (const auto &c : raw_value) {
    hex_stream << std::setw(2) << (0x00ff & static_cast<const uint16_t>(c));
  }

  return hex_stream.str();
}

bool Code::operator==(const Code& code) const {
  return this->value() == code.value();
}

const Code Code::operator+(const Code& code) const {
  return Code(this->value() + code.value());
}

Code& Code::operator+=(const Code& code) {
  *this = *this + code;
  return *this;
}

}  // namespace little_crypt
