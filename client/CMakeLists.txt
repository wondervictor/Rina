# CMake Version
cmake_minimum_required(VERSION 2.8)

project(client)

set(CMAKE_CXX_STANDARD 11)

include_directories(../)
include_directories(../thread/)
include_directories(../socket/)
set(SRC client.cpp test_main.cpp ../Model.cpp)

add_executable(client ${SRC})

set(DIR_PATH /Users/Vic/Dev/System/Rina)
set(LIB ${DIR_PATH}/socket/librsocket.a ${DIR_PATH}/thread/librthread.a)

target_link_libraries(client ${LIB})