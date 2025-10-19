cmake_minimum_required(VERSION 3.13)
project(test_package)

add_executable(test_package test.cpp)

target_include_directories(test_package PRIVATE 
    "${CMAKE_CURRENT_LIST_DIR}/../../include"
)