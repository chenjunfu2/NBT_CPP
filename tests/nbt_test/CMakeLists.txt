cmake_minimum_required(VERSION 3.13)
project(nbt_test LANGUAGES CXX)

add_executable(nbt_test
    nbt_test.cpp
)

target_link_libraries(nbt_test
    PUBLIC
        ${COMMON_LIBS}
)