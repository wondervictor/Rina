# CMake Version

cmake_minimum_required(VERSION 2.8)

project(Rina)

execute_process(COMMAND mkdir bin)
execute_process(COMMAND mkdir lib)

set(CMAKE_CXX_STANDARD 11)

set(BIN bin)
set(LIB_DIR lib)


set(SRC_DIR src)

find_package(RapidJSON)

set(LIBRARY_OUTPUT_PATH ${LIB_DIR})
set(EXECUTABLE_OUTPUT_PATH ${BIN})

# socket
add_library(rsocket ${SRC_DIR}/socket.cpp)
# thread
add_library(rthread ${SRC_DIR}/thread.cpp)

#link_directories(${LIB_DIR})

add_executable(socket_test ${SRC_DIR}/socket_test.cpp)
target_link_libraries(socket_test rsocket.a)

# thread

add_executable(thread_test ${SRC_DIR}/thread_test.cpp)

target_link_libraries(thread_test rthread.a)
# LIBS
set(LIBS rsocket.a rthread.a)

# server

set(SERVER_SRC ${SRC_DIR}/server.cpp ${SRC_DIR}/model.cpp ${SRC_DIR}/server_test.cpp)
add_executable(server ${SERVER_SRC})
target_link_libraries(server ${LIBS})

# client

set(CLIENT_SRC ${SRC_DIR}/client.cpp ${SRC_DIR}/client_test.cpp ${SRC_DIR}/model.cpp)
add_executable(client ${CLIENT_SRC})
target_link_libraries(client ${LIBS})



