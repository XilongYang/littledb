cmake_minimum_required(VERSION 3.16)
project(littledb)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(base)
include_directories(db)
include_directories(third_party)

add_library(littledb
        include/littledb.h
        base/byte_string.cc
        base/code.cc
        base/sha256.cc
        base/hmac.cc
        base/aes128.cc
        base/rsa1024.cc
        db/mempool.cc
        db/mempool.h
        db/skip_list.cc
        db/skip_list.h
        db/inner_key.cc
        db/inner_key.h
        db/memtable.cc
        db/memtable.h base/request_protocol.cc base/response_protocol.cc)

enable_testing()
add_subdirectory(base/test)
add_subdirectory(db/test)
