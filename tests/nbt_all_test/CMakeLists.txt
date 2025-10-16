cmake_minimum_required(VERSION 3.13)
project(nbt_all_test LANGUAGES CXX)

target_link_libraries(nbt_all_test
    PUBLIC
        xxhash
        zlib
)

add_executable(nbt_all_test
    nbt_all_test.cpp
)

target_compile_definitions(nbt_all_test
    PUBLIC
        $<$<STREQUAL:${CMAKE_CXX_COMPILER_ARCHITECTURE_ID},X86>:WIN32>
)