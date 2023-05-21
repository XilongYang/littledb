// Created by Xilong Yang on 2022-03-11.
//

#ifndef LITTLEDB_H
#define LITTLEDB_H

#include <functional>
#include <stdexcept>
#include <string>
#include <iostream>
#include <memory>

namespace littledb {

/** Using std::size_t as our size type. **/
using Size = std::size_t;

/** Using the 8bit unsigned int for Byte type. **/
using Byte = uint8_t;

/** Code: A type used by base functions.
 * It can be constructed with C style string, std::string and a pair of (size, byte).
 * It supports copy and move operations.
 * It has a iterator class named "CodeIter", so that the class can support the range-based for loop.
 */
class Code {
 public:
  Code();
  ~Code();
  Code(Code const &code);
  Code(Code &&code);
  Code(Size size, Byte byte);
  Code(char* str, Size size);
  explicit Code(const std::string& str);

  [[nodiscard]] Size ByteSum() const;

  // Judge if the inner codes are same.
//  bool operator==(const Code& code) const;
//  bool operator!=(const Code& code) const;

  // Append 'code' to the end of the object.
//  const Code operator+(const Code& code) const;
//  Code& operator+=(const Code& code);

  /** The function provides a transform from inner types to Code.
   *  Note it only worked for the top type, that means
   *  if you pass a pointer in the function, it will only transform
   *  the pointer itself rather than the object that the pointer referenced.
   *  For example:
   *    BaseToCode(int i) will return a Code which include 4 Bytes(depends on
   *      system) which represents the code of i.
   *    BaseToCode(char *str) will return a Code which also include 4 Bytes(depends
   *      on system) which represents the code of the pointer(not the contents).
   */
//  template <typename T>
//  static Code BaseToCode(const T& target) {
//    ByteString result;
//    auto p = reinterpret_cast<const Byte*>(&target);
//    for (int i = 0; i < sizeof(T); ++i) {
//      result.push_back(*(p + i));
//    }
//    return Code(result);
//  }
 private:
    class CodeImpl;
    std::unique_ptr<CodeImpl> impl_;
};  // class Code



//// The interface makes implements can be encoded or decoded.
//class CodableInterface {
// public:
//  virtual Code Encode() = 0;
//  virtual void Decode(const Code& c) = 0;
//};
//
//// Exception class using for errors from CodableInterface.Decode()
//// or Code::FromDec() or Code::FromHex()
//class DecodeError : public std::logic_error {
// public:
//  explicit DecodeError(const std::string& s) : std::logic_error(s) {}
//};
//
//// The following functions provides switch between little endian and big endian.
//
//// Judge if the system is big endian.
//inline bool SystemIsBigEndian() {
//  uint16_t test = 0x1234;
//  uint8_t first = *reinterpret_cast<uint8_t*>(&test);
//  return first == 0x12;
//}
//
//// Switch between little endian and big endian.
//template<typename T>
//T SwitchEndian(const T& input) {
//  T output(0);
//  constexpr size_t size = sizeof(input);
//  char* data = reinterpret_cast<char*>(&output);
//  for (size_t i = 0; i < size; ++i) {
//    data[i] = input >> ((size - i - 1) * 8);
//  }
//  return output;
//}
//
//// Take the big endian represent of a value.
//template<typename T>
//T BigEndian(const T& input) {
//  if (SystemIsBigEndian()) {
//    return input;
//  }
//  return SwitchEndian(input);
//}
//
//// Take the little endian representation of a value.
//template<typename T>
//T LittleEndian(const T& input) {
//  if (SystemIsBigEndian()) {
//    return SwitchEndian(input);
//  }
//  return input;
//}
//
//// Take the local represent of a big endian value.
//// Note that the argument MUST BE a big endian representation.
//template<typename T>
//T LocalEndian(const T& input) {
//  if (SystemIsBigEndian()) {
//    return input;
//  }
//  return SwitchEndian(input);
//}
//
//// Take the SHA256 hash for a message.
//Code Sha256(const Code& message);
//
//// Take the hash for a message using specified hash algorithm and a key message.
//// The hash function is SHA256 on default.
//// To specify a hash algorithm, you must provide a hash function and a num
//// which is the chunk size of this algorithm.
//Code Hmac(const Code& message, const Code& key
//          , std::pair<std::function<Code(const Code&)>, size_t> hash
//          = {Sha256, 64});
//
//// Take the ciphertext for a pair of a plaintext and a key using AES128 algorithm.
//// The key will be processed by SHA256 algorithm and truncate to 128bit.
//// If the text length (by bits) is not divisible by 128, it will be padded
//// with 0 at the end till the length is divisible by 128.
//Code Aes128Encrypt(const Code& plaintext, const Code& key);
//
//// Take the plaintext for a pair of a ciphertext and a key using AES128 algorithm.
//// The key will be processed by SHA256 algorithm and truncate to 128bit.
//// If the text length (by bits) is not divisible by 128, the function will throw
//// an exception.
//Code Aes128Decrypt(const Code& ciphertext, const Code& key);
//
//// Take the RSA processed text for a pair of a text and key using RSA algorithm.
//// If the text length is less than 1024bit, it will be padded with
//// 0 at the beginning till the length is divisible by 1024.
//// If the text length is bigger than 1024bit, the function will throw an
//// exception.
//Code Rsa1024(const Code& text, const Code& exponent, const Code& modulus);
//
//// Exception class using for errors form Aes128Decrypt().
//class DecryptError : public std::logic_error {
// public:
//  explicit DecryptError(const std::string &s) : std::logic_error(s) {}
//};
//
//// Network Protocol
//
//class RequestProtocol : public CodableInterface{
// public:
//  RequestProtocol(const Code& user_token, Byte opt
//          , const Code& data_key = Code(), const Code& data_ = Code());
//
//  Code Encode() override;
//  void Decode(const Code& code) override;
//
//  Code UserToken() const;
//  Byte Option() const;
//  size_t DataLength() const;
//  Code DataKey() const;
//  Code Data() const;
//  Code HmacKey() const;
//  Code HmacCode() const;
// private:
//  Code user_token_;
//  Byte opt_;
//  Byte padding_[7];
//  size_t data_length_;
//  Code data_key_;
//  Code data_;
//  Code hmac_;
//};
//
//class ResponseProtocol : public CodableInterface {
// public:
//  ResponseProtocol(size_t status_, const Code& hmac_key
//                   , const Code& data = Code());
//
//  Code Encode() override;
//  void Decode(const Code& code) override;
//
//  size_t Status() const;
//  size_t DataLength() const;
//  Code Data() const;
//  Code HmacCode() const;
// private:
//  size_t status_;
//  size_t data_length_;
//  Code data_;
//  Code hmac_;
//};
//
}  // namespace littledb
//
//// I/O for ByteString.
//std::ostream& operator<<(std::ostream& os
//                         , const littledb::ByteString& byte_str);
//
//std::istream& operator>>(std::istream& is
//                         , littledb::ByteString& byte_str);

#endif  // LITTLEDB_H