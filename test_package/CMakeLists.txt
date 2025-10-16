cmake_minimum_required(VERSION 3.13)
project(test_package LANGUAGES CXX)

add_executable(test_package
    test.cpp
)

target_link_libraries(test_package
    PUBLIC
        ${COMMON_LIBS}
)

target_compile_definitions(test_package
    PUBLIC
        $<$<STREQUAL:${CMAKE_CXX_COMPILER_ARCHITECTURE_ID},X86>:WIN32>
)