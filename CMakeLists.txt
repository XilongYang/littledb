cmake_minimum_required(VERSION 3.21)
project(little_crypt)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(src)

add_library(little_crypt
        include/littledb.h
        src/code_impl.cc
        src/sha256.cc
        src/hmac.cc
        src/aes128.cc
        )

enable_testing()
add_subdirectory(test)