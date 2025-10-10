cmake_minimum_required(VERSION 3.13)
project(test_package LANGUAGES CXX)

add_executable(test_package
    test.cpp
)

target_compile_features(test_package
    PUBLIC
        cxx_std_20
        c_std_17
)

target_compile_definitions(test_package
    PUBLIC
        _CONSOLE
        UNICODE
        _UNICODE
        $<$<CONFIG:Debug>:_DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
        $<$<STREQUAL:${CMAKE_CXX_COMPILER_ARCHITECTURE_ID},X86>:WIN32>
)

target_link_libraries(test_package
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

target_compile_options(test_package
    PUBLIC
        /W3
)

