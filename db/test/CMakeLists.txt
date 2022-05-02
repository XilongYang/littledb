cmake_minimum_required(VERSION 3.16)
project(littledb)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 17)

enable_testing()

add_executable(db_test
        inner_key_test.cc
        mempool_test.cc
        skip_list_test.cc
        memtable_test.cc)

target_link_libraries(db_test
        littledb
        ${GTEST_BOTH_LIBRARIES}
        pthread)

gtest_discover_tests(db_test)
