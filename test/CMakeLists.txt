cmake_minimum_required(VERSION 3.21)
project(little_crypt)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

enable_testing()

add_executable(code_impl_test
        code_impl_test.cc)

target_link_libraries(code_impl_test
        little_crypt
        ${GTEST_BOTH_LIBRARIES}
        pthread)

add_executable(switch_endian_test
        switch_endian_test.cc)

target_link_libraries(switch_endian_test
        little_crypt
        ${GTEST_BOTH_LIBRARIES}
        pthread)

add_executable(sha256_test
        sha256_test.cc)

target_link_libraries(sha256_test
        little_crypt
        ${GTEST_BOTH_LIBRARIES}
        pthread)

add_executable(hmac_test
        hmac_test.cc)

target_link_libraries(hmac_test
        little_crypt
        ${GTEST_BOTH_LIBRARIES}
        pthread)

add_executable(aes128_test
        aes128_test.cc)

target_link_libraries(aes128_test
        little_crypt
        ${GTEST_BOTH_LIBRARIES}
        pthread)

gtest_discover_tests(
        code_impl_test
        switch_endian_test
        sha256_test
        hmac_test
        aes128_test
)
