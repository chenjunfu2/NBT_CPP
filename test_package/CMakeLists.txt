cmake_minimum_required(VERSION 3.13)
project(test_package)

find_package(nbt_cpp REQUIRED)

add_executable(test_package test.cpp)
target_link_libraries(test_package PRIVATE nbt_cpp)