// Created by Xilong Yang on 2022-03-11.
//

#ifndef LITTLEDB_H
#define LITTLEDB_H

#include <functional>
#include <stdexcept>
#include <string>

namespace littledb{
// Using the 8bit unsigned int for Byte type.
using Byte = uint8_t;

// Using the string which contains 8bit unsigned int for ByteString type.
using ByteString = std::basic_string<Byte>;

// Using std::size_t as our size type.
using std::size_t;

ByteString ToByteString(const std::string& str);
ByteString ToByteString(const char* c_ptr, size_t size);

// The class provides a type which can be used by crypt functions.
// The class can be constructed with std::string.
// The class supports copy and move operations.
// Use code.value() to get a std::string.
// Use + operator to connect 2 or more Codes.
// Use Code::BaseToCode() to convert base type to code.
class Code {
 public:
  explicit Code(const ByteString& bytes = ByteString());

  // Get the inner value by a std::string.
  [[nodiscard]] const ByteString& value() const;

  // Get the hex representation of the inner value by a Byte[].
  [[nodiscard]] ByteString HexValue() const;

  // Judge if the inner codes are same.
  bool operator==(const Code& code) const;
  bool operator!=(const Code& code) const;

  // Append 'code' to the end of the object.
  const Code operator+(const Code& code) const;

  Code& operator+=(const Code& code);

  // The function provides a transform from inner types to Code.
  // Note it only worked for the top type, that means
  // if you pass a pointer in the function, it will only transform
  // the pointer itself rather than the object that the pointer referenced.
  // For example:
  // BaseToCode(int i) will return a Code which include 4 bytes(depends on
  // system) that is the code of i.
  // BaseToCode(char *str) will return a Code which also include 4 bytes(depends
  // on system) that is the code of the pointer.
  template <typename T>
  static Code BaseToCode(const T& target) {
    ByteString result;
    auto p = reinterpret_cast<const Byte*>(&target);
    for (int i = 0; i < sizeof(T); ++i) {
      result.push_back(*(p + i));
    }
    return Code(result);
  }

 private:
  ByteString value_;
};  // class Code



// The interface makes implements can be encoded or decoded.
class CodableInterface {
 public:
  virtual Code Encode() = 0;
  virtual void Decode(const Code& c) = 0;
};

// Exception class using for errors from CodableInterface.Decode()
class DecodeError : public std::logic_error {
 public:
  explicit DecodeError(const std::string& s) : std::logic_error(s) {}
};

// The following functions provides switch between small endian and big endian.

// Judge if the system is big endian.
inline bool SystemIsBigEndian() {
  uint16_t test = 0x1234;
  uint8_t first = *reinterpret_cast<uint8_t*>(&test);
  return first == 0x12;
}

// Switch between small endian and big endian.
template<typename T>
T SwitchEndian(const T& input) {
  T output(0);
  constexpr size_t size = sizeof(input);
  char* data = reinterpret_cast<char*>(&output);
  for (size_t i = 0; i < size; ++i) {
    data[i] = input >> ((size - i - 1) * 8);
  }
  return output;
}

// Take the big endian represent of a value.
template<typename T>
T BigEndian(const T& input) {
  if (SystemIsBigEndian()) {
    return input;
  }
  return SwitchEndian(input);
}

// Take the small endian representation of a value.
template<typename T>
T SmallEndian(const T& input) {
  if (SystemIsBigEndian()) {
    return SwitchEndian(input);
  }
  return input;
}

// Take the local represent of a big endian value.
// Note that the argument MUST BE a big endian representation.
template<typename T>
T LocalEndian(const T& input) {
  if (SystemIsBigEndian()) {
    return input;
  }
  return SwitchEndian(input);
}

// Take the SHA256 hash for a message.
Code Sha256(const Code& message);

// Take the hash for a message using specified hash algorithm and a key message.
// The hash function is SHA256 on default.
// To specify a hash algorithm, you must provide a hash function and a num
// which is the chunk size of this algorithm.
Code Hmac(const Code& message, const Code& key
          , std::pair<std::function<Code(const Code&)>, size_t> hash
          = {Sha256, 64});

// Take the ciphertext for a pair of a plaintext and a key using AES128 algorithm.
// The key will be processed by SHA256 algorithm and truncate to 128bit.
// If the text length (by bits) is not divisible by 128, it will be padded
// with 0 at the end till the length is divisible by 128.
Code Aes128Encrypt(const Code& plaintext, const Code& key);

// Take the plaintext for a pair of a ciphertext and a key using AES128 algorithm.
// The key will be processed by SHA256 algorithm and truncate to 128bit.
// If the text length (by bits) is not divisible by 128, the function will throw
// an exception.
Code Aes128Decrypt(const Code& ciphertext, const Code& key);

// Exception class using for errors form Aes128Decrypt()
class DecryptError : public std::logic_error {
  explicit DecryptError(const std::string &s) : std::logic_error(s) {}
};

}  // namespace littledb

#endif  // LITTLEDB_H