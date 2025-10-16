cmake_minimum_required(VERSION 3.13)
project(xxhash LANGUAGES CXX)

add_library(xxhash
    STATIC
        xxh_x86dispatch.c
        xxhash.c
)

target_compile_definitions(xxhash
    PUBLIC
        $<$<STREQUAL:${CMAKE_CXX_COMPILER_ARCHITECTURE_ID},X86>:WIN32>
)