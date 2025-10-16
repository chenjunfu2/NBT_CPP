cmake_minimum_required(VERSION 3.13)
project(NBT_CPP)

#全局 C++/C 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD_REQUIRED ON)

#全局头文件目录
include_directories(
    ${CMAKE_SOURCE_DIR}/deps/gzip
    ${CMAKE_SOURCE_DIR}/deps/zlib
    ${CMAKE_SOURCE_DIR}/deps/xxhash
    ${CMAKE_SOURCE_DIR}/deps
    ${CMAKE_SOURCE_DIR}/include
)

#公共编译选项
if(MSVC)
    add_compile_options(/W3)
else()
    add_compile_options(-Wall -Wextra)
endif()

#公共编译定义
add_definitions(-D_CONSOLE -DUNICODE -D_UNICODE)

#不同配置下的定义
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

#公共库变量（方便复用）
set(COMMON_LIBS xxhash zlib)


#添加库与项目

#库项目
add_subdirectory(deps/xxhash)
add_subdirectory(deps/zlib)

#测试项目
add_subdirectory(tests/endian_test)
add_subdirectory(tests/mutf8_test)
add_subdirectory(tests/nbt_all_test)
add_subdirectory(tests/nbt_benchmark)
add_subdirectory(tests/nbt_test)

#包测试
add_subdirectory(test_package)

#使用案例
add_subdirectory(usage)
