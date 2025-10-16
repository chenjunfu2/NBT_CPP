cmake_minimum_required(VERSION 3.13)
project(NBT_CPP)

#deps
add_subdirectory(deps/xxhash)
add_subdirectory(deps/zlib)

#tests
add_subdirectory(tests/endian_test)
add_subdirectory(tests/mutf8_test)
add_subdirectory(tests/nbt_all_test)
add_subdirectory(tests/nbt_benchmark)
add_subdirectory(tests/nbt_test)

#test_package
add_subdirectory(test_package)

#usage
add_subdirectory(usage)
