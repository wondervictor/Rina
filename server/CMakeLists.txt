# CMake Version
cmake_minimum_required(VERSION 2.8)

project(server)

set(CMAKE_CXX_STANDARD 11)

set(SRC server.cpp test_main.cpp)

add_executable(server ${SRC})

set(LIB ../socket/librsocket ../thread/librthread)

target_link_libraries(server ${LIB})