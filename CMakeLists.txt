cmake_minimum_required(VERSION 3.17)
project(protei_task)

set(CMAKE_CXX_STANDARD 14)

add_executable(server server.cpp Socket.cpp Socket.h Server.cpp Server.cpp)

add_executable(client client.cpp Socket.cpp Socket.h)
