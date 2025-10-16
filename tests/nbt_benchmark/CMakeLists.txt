cmake_minimum_required(VERSION 3.13)
project(nbt_benchmark LANGUAGES CXX)

add_executable(nbt_benchmark
    nbt_benchmark.cpp
)

target_link_libraries(nbt_benchmark
    PUBLIC
        ${COMMON_LIBS}
)

target_compile_definitions(nbt_benchmark
    PUBLIC
        $<$<STREQUAL:${CMAKE_CXX_COMPILER_ARCHITECTURE_ID},X86>:WIN32>
)