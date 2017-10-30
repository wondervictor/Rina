# CMake Version
cmake_minimum_required(VERSION 2.8)

project(client)

set(CMAKE_CXX_STANDARD 11)

set(SRC client.cpp test_main.cpp)

add_executable(client ${SRC})

set(LIB ../socket/librsocket ../thread/librthread)

target_link_libraries(client ${LIB})