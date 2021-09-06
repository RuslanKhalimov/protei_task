include_directories(..)

add_executable(server
        server.cpp
        Epoll.cpp
        Epoll.h
        ../Socket.cpp
        ../Socket.h
        Server.cpp
        Server.cpp)
