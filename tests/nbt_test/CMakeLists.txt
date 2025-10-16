cmake_minimum_required(VERSION 3.13)
project(nbt_test LANGUAGES CXX)

target_link_libraries(nbt_test
    PUBLIC
        xxhash
        zlib
)

add_executable(nbt_test
    nbt_test.cpp
)

target_compile_definitions(nbt_test
    PUBLIC
        $<$<STREQUAL:${CMAKE_CXX_COMPILER_ARCHITECTURE_ID},X86>:WIN32>
)