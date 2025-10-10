cmake_minimum_required(VERSION 3.13)
project(xxhash LANGUAGES CXX)


add_executable(xxhash
    xxh_x86dispatch.c
    xxhash.c
)

target_compile_features(xxhash
    PUBLIC
        cxx_std_20
        c_std_17
)

target_compile_definitions(xxhash
    PUBLIC
        _CONSOLE
        UNICODE
        _UNICODE
        $<$<CONFIG:Debug>:_DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
        $<$<STREQUAL:$<CMAKE_CXX_COMPILER_ARCHITECTURE_ID>,X86>:WIN32>
)

target_compile_options(xxhash
    PUBLIC
        /W3
)

