cmake_minimum_required(VERSION 3.13)
project(endian_test LANGUAGES CXX)

add_executable(endian_test
    endian_test.cpp
)

target_link_libraries(endian_test
    PUBLIC
        ${COMMON_LIBS}
)