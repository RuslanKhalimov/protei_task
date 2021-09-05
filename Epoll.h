#pragma once

#include <sys/epoll.h>
#include <queue>

class Epoll
{
public:
  ~Epoll();
  void init(int serverSocketFd);
  void addSocketFd(int socketFd);
  void removeSocketFd(int socketFd);
  int getNextEventFd();

private:
  static const size_t MAX_EVENTS = 10;
  int epollfd_ = -1;
  struct epoll_event nextEvent_{}, mainEvent_{};
  std::queue<struct epoll_event> eventQueue_;
};
