cmake_minimum_required(VERSION 3.16)
project(little_crypt)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

enable_testing()

add_executable(all_test
        code_impl_test.cc
        switch_endian_test.cc
        sha256_test.cc
        hmac_test.cc
        aes128_test.cc
        )

target_link_libraries(all_test
        little_crypt
        ${GTEST_BOTH_LIBRARIES}
        pthread)

gtest_discover_tests(all_test)
