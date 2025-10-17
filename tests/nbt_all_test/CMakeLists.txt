cmake_minimum_required(VERSION 3.13)
project(nbt_all_test LANGUAGES CXX)

add_executable(nbt_all_test
    nbt_all_test.cpp
)

target_link_libraries(nbt_all_test
    PUBLIC
        ${COMMON_LIBS}
)