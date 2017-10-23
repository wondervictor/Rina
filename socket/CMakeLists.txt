# CMake Version

cmake_minimum_required(VERSION 2.8)

project(socket_test)
set(CMAKE_CXX_STANDARD 11)


set(DIR_LIB_SRCS socket.cpp)

#aux_source_directory(. DIR_LIB_SRCS)
add_library(rsocket ${DIR_LIB_SRCS})

add_executable(socket_test test_main.cpp)

target_link_libraries(socket_test rsocket)





