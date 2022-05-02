cmake_minimum_required(VERSION 3.16)
project(littledb)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 17)

enable_testing()

add_executable(crypt_test
        code_impl_test.cc
        switch_endian_test.cc
        sha256_test.cc
        hmac_test.cc
        aes128_test.cc
        byte_string.cc
        rsa_test.cc)

target_link_libraries(crypt_test
        littledb
        ${GTEST_BOTH_LIBRARIES}
        pthread)

gtest_discover_tests(crypt_test)
