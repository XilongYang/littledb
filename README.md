# LittleDB

![license](https://img.shields.io/github/license/XilongYang/littledb?style=flat-square)
![cmake](https://img.shields.io/badge/CMake%20Version-%3E%3D3.16-green?style=flat-square)
![cpp](https://img.shields.io/badge/C%2B%2B%20Version-14-green?style=flat-square)
![gtest](https://img.shields.io/badge/Googletest-v1.10-green?style=flat-square)
![build](https://img.shields.io/github/actions/workflow/status/XilongYang/littledb/cmake.yml?style=flat-square)

A simple C++ library provides an embedded K-V database and some encrypt/decrypt concerned functions.

## Build

Just do this, make sure you had installed the Googletest library:

```bash
git clone https://github.com/XilongYang/littledb.git
cd littledb
mkdir -p build && cd build
cmake .. .
make
```

To test the library, you can simply do this:

```bash
cd build/test
./all_test
```

## Usage

This project is a static library. So you can use it by simply copy the `include\littledb.h` to you project include 
path and link the `liblittledb.a` to you project.

For example:

```C++
//Test
//├── main.cc
//└── third_party
//    ├── include
//    │   └── littledb.h
//    └── lib
//        └── liblittledb.a

//main.cc

#include "third_party/include/littledb.h"
#include <iostream>

int main() {
    littledb::ByteString str(littledb::ToByteString("Hello"));
    std::cout << str << std::endl;

    littledb::Code code(str);
    code = littledb::Sha256(code);

    std::cout << code.HexValue() << std::endl;
    return 0;
}
```

```bash
# workdir=Test
g++ -o main main.cc -Lthird_party/lib -llittledb
./main
```

```bash
# output
Hello
185f8db32271fe25f561a6fc938b2e264306ec304eda518007d1764826381969
```

## Interfaces

### ByteString: 
Using the string which contains 8bit unsigned int for `ByteString` type.
It's usage same to `std::string`.

### ToByteString:

Convert a `std::string` or `C style string` to the `ByteString` class.

```C++
ByteString ToByteString(const std::string& str);
ByteString ToByteString(const char* c_ptr, size_t size);
```

### Code:

The class provides a type which can be used by crypt functions.

It can be constructed with `ByteString`. 

It supports copy and move operations.

Use `code.value()` to get a `ByteString` contains the value of the `Code`.

Use `+` operator to connect 2 or more `Code`.

Use `Code::BaseToCode()` to convert base type to code.

### Codable:

The interface makes implements can be encoded or decoded.

It contains two pure virtual function:

```C++
virtual Code Encode();
virtual void Decode(const Code& c);
```

Where the `Encode` function convert an object to `Code`, and the `Decode` function changes the object's value which 
contained by a `Code`.

### Endian functions

The following functions provides switch between little endian and big endian.

```C++
// Judge if the system is big endian.
bool SystemIsBigEndian();

// Switch between little endian and big endian.
template<typename T>
T SwitchEndian(const T& input);


// Take the big endian represent of a value.
template<typename T>
T BigEndian(const T& input); 

// Take the little endian representation of a value.
template<typename T>
T LittleEndian(const T& input);

// Take the local represent of a big endian value.
// Note that the argument MUST BE a big endian representation.
template<typename T>
T LocalEndian(const T& input); 
```

### SHA-256

```C++
// Take the SHA256 hash for a message.
Code Sha256(const Code& message);
```

### HMAC

```C++
// Take the hash for a message using specified hash algorithm and a key message.
// The hash function is SHA256 on default.
// To specify a hash algorithm, you must provide a hash function and a num
// which is the chunk size of this algorithm.
Code Hmac(const Code& message, const Code& key
          , std::pair<std::function<Code(const Code&)>, size_t> hash
          = {Sha256, 64});
```

### Aes128

```C++
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
 public:
  explicit DecryptError(const std::string &s) : std::logic_error(s) {}
};
```
