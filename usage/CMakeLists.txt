cmake_minimum_required(VERSION 3.13)
project(usage LANGUAGES CXX)

target_link_libraries(usage
    PUBLIC
        xxhash
        zlib
)

add_executable(usage
    example.cpp
)

target_compile_definitions(usage
    PUBLIC
        $<$<STREQUAL:${CMAKE_CXX_COMPILER_ARCHITECTURE_ID},X86>:WIN32>
)