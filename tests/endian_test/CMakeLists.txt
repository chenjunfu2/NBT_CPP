cmake_minimum_required(VERSION 3.13)
project(endian_test LANGUAGES CXX)

add_executable(endian_test
    endian_test.cpp
)

target_compile_features(endian_test
    PUBLIC
        cxx_std_20
        c_std_17
)

target_compile_definitions(endian_test
    PUBLIC
        _CONSOLE
        UNICODE
        _UNICODE
        $<$<CONFIG:Debug>:_DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
        $<$<STREQUAL:${CMAKE_CXX_COMPILER_ARCHITECTURE_ID},X86>:WIN32>
)

target_link_libraries(endian_test
    PUBLIC
        xxhash
        zlib
)

include_directories(
    ${CMAKE_SOURCE_DIR}/deps/gzip
    ${CMAKE_SOURCE_DIR}/deps/zlib
    ${CMAKE_SOURCE_DIR}/deps/xxhash
    ${CMAKE_SOURCE_DIR}/deps
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_options(endian_test
    PUBLIC
        /W3
)

