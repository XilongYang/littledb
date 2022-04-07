# Little Crypt

![license](https://img.shields.io/github/license/XilongYang/little_crypt)
![cmake](https://img.shields.io/badge/CMake%20Version-%3E%3D3.16-green)
![cpp](https://img.shields.io/badge/C%2B%2B%20Version-17-green)
![gtest](https://img.shields.io/badge/Googletest-v1.10-green)
![build](https://img.shields.io/github/workflow/status/XilongYang/little_crypt/CMake)

A simple C++ library provides some encrypt/decrypt concerned interfaces and functions such as SHA, HMAC and AES.

## build

Just do this, make sure you had installed the Googletest library:

```bash
git clone https://github.com/XilongYang/little_crypt.git
cd little_crypt
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

This project is a static library. So you can use it with simply copy the `include\little_crypt.h` to you project include 
path and link the `liblittle_crypt.a` to you project.

For example:

```bash

```