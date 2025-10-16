cmake_minimum_required(VERSION 3.13)
project(endian_test LANGUAGES CXX)

add_executable(endian_test
    endian_test.cpp
)

target_link_libraries(endian_test
    PUBLIC
        ${COMMON_LIBS}
)

target_compile_definitions(endian_test
    PUBLIC
        $<$<STREQUAL:${CMAKE_CXX_COMPILER_ARCHITECTURE_ID},X86>:WIN32>
)