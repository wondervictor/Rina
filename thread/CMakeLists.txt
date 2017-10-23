# CMake Version

cmake_minimum_required(VERSION 2.8)

project(test_thread)
set(CMAKE_CXX_STANDARD 11)

set(DIR_LIB_SRCS thread.cpp)

add_library(rthread ${DIR_LIB_SRCS})

add_executable(socket_test test_thread.cpp)

target_link_libraries(test_thread rthread)