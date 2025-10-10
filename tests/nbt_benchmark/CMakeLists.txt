cmake_minimum_required(VERSION 3.13)
project(nbt_benchmark LANGUAGES CXX)


add_executable(nbt_benchmark
    nbt_benchmark.cpp
)

target_compile_features(nbt_benchmark
    PUBLIC
        cxx_std_20
        c_std_17
)

target_compile_definitions(nbt_benchmark
    PUBLIC
        _CONSOLE
        UNICODE
        _UNICODE
        $<$<CONFIG:Debug>:_DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
        $<$<STREQUAL:${CMAKE_CXX_COMPILER_ARCHITECTURE_ID},X86>:WIN32>
)

target_link_libraries(nbt_benchmark
    PUBLIC
        xxhash
        zlib
)

target_compile_options(nbt_benchmark
    PUBLIC
        /W3
)

