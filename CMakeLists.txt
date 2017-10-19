# CMake Version

cmake_minimum_required(VERSION 2.8)

project(Rina)

add_subdirectory(socket)

aux_source_directory(. DIR_SRDS)

add_executable(Rina main.cpp)

target_link_libraries(Rina Socket)