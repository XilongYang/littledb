cmake_minimum_required(VERSION 3.20)

project(LittleDB VERSION 0.0.1 LANGUAGES C)

enable_testing()

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_COMPILER clang)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_compile_options(-Wall -Wextra -Wpedantic)

add_library(littledb STATIC
    src/littledb.c
)

target_include_directories(littledb
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_executable(test_runner
    tests/test_main.c
    third_party/unity/unity.c
)

target_include_directories(test_runner
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/unity
)

target_link_libraries(test_runner
    PRIVATE
        littledb
)

add_test(NAME test_runner COMMAND test_runner)

