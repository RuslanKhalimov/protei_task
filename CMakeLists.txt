cmake_minimum_required(VERSION 3.17)
project(protei_task)

set(CMAKE_CXX_STANDARD 14)

add_executable(server
        server.cpp
        Epoll.cpp
        Epoll.h
        Socket.cpp
        Socket.h
        Server.cpp
        Server.cpp)

add_executable(client
        client.cpp
        Client.cpp
        Client.h
        Socket.cpp
        Socket.h)
