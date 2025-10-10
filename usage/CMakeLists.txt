cmake_minimum_required(VERSION 3.13)
project(usage LANGUAGES CXX)

add_executable(usage
    example.cpp
)

target_compile_features(usage
    PUBLIC
        cxx_std_20
        c_std_17
)

target_compile_definitions(usage
    PUBLIC
        _CONSOLE
        UNICODE
        _UNICODE
        $<$<CONFIG:Debug>:_DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
        $<$<STREQUAL:${CMAKE_CXX_COMPILER_ARCHITECTURE_ID},X86>:WIN32>
)

target_link_libraries(usage
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

target_compile_options(usage
    PUBLIC
        /W3
)

