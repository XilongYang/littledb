cmake_minimum_required(VERSION 3.16)
project(littledb)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(crypt)
include_directories(db)
include_directories(third_party)

add_library(littledb
        include/littledb.h
        crypt/byte_string.cc
        crypt/code_impl.cc
        crypt/sha256.cc
        crypt/hmac.cc
        crypt/aes128.cc
        crypt/rsa1024.cc
        db/mempool.cc
        db/mempool.h
        db/skip_list.cc
        db/skip_list.h
        db/inner_key.cc
        db/inner_key.h
        db/memtable.cc
        db/memtable.h)

enable_testing()
add_subdirectory(crypt/test)
add_subdirectory(db/test)
