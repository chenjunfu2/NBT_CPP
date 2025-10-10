cmake_minimum_required(VERSION 3.13)
project(mutf8_test LANGUAGES CXX)


add_executable(mutf8_test
    mutf8_test.cpp
)

target_compile_features(mutf8_test
    PUBLIC
        cxx_std_20
        c_std_17
)

target_compile_definitions(mutf8_test
    PUBLIC
        _CONSOLE
        UNICODE
        _UNICODE
        $<$<CONFIG:Debug>:_DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
        $<$<STREQUAL:${CMAKE_CXX_COMPILER_ARCHITECTURE_ID},X86>:WIN32>
)

target_link_libraries(mutf8_test
    PUBLIC
        xxhash
        zlib
)

target_compile_options(mutf8_test
    PUBLIC
        /W3
)

