# CMake Version

cmake_minimum_required(VERSION 2.8)

project(Rina)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(client)
add_subdirectory(socket)
add_subdirectory(thread)
add_subdirectory(server)

aux_source_directory(. DIR_SRDS)

add_executable(Rina main.cpp)

target_link_libraries(Rina Socket)