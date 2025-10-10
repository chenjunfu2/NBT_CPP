cmake_minimum_required(VERSION 3.13)
project(NBT_CPP)

add_subdirectory(deps/xxhash)
add_subdirectory(deps/zlib)
add_subdirectory(tests/endian_test)
add_subdirectory(tests/mutf8_test)
add_subdirectory(tests/nbt_all_test)
add_subdirectory(tests/nbt_benchmark)
add_subdirectory(tests/nbt_test)
add_subdirectory(usage)

include_directories(
    ${CMAKE_SOURCE_DIR}/deps/gzip
    ${CMAKE_SOURCE_DIR}/deps/zlib
    ${CMAKE_SOURCE_DIR}/deps/xxhash
    ${CMAKE_SOURCE_DIR}/deps
    ${CMAKE_SOURCE_DIR}/include
)