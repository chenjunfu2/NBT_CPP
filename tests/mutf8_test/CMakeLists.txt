cmake_minimum_required(VERSION 3.13)
project(mutf8_test LANGUAGES CXX)

add_executable(mutf8_test
    mutf8_test.cpp
)

target_link_libraries(mutf8_test
    PUBLIC
        ${COMMON_LIBS}
)

target_compile_definitions(mutf8_test
    PUBLIC
        $<$<STREQUAL:${CMAKE_CXX_COMPILER_ARCHITECTURE_ID},X86>:WIN32>
)