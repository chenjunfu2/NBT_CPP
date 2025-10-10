cmake_minimum_required(VERSION 3.13)
project(nbt_all_test LANGUAGES CXX)


add_executable(nbt_all_test
    nbt_all_test.cpp
)

target_compile_features(nbt_all_test
    PUBLIC
        cxx_std_20
        c_std_17
)

target_compile_definitions(nbt_all_test
    PUBLIC
        _CONSOLE
        UNICODE
        _UNICODE
        $<$<CONFIG:Debug>:_DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
        $<$<STREQUAL:$<CMAKE_CXX_COMPILER_ARCHITECTURE_ID>,X86>:WIN32>
)

target_link_libraries(nbt_all_test
    PUBLIC
        xxhash
        zlib
)

target_compile_options(nbt_all_test
    PUBLIC
        /W3
)

