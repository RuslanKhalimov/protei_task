#include "Epoll.h"
#include <unistd.h>
#include <stdexcept>
#include <cstring>


Epoll::~Epoll()
{
  if (epollfd_ != -1) {
    ::close(epollfd_);
  }
}

void Epoll::init(int serverSocketFd)
{
  epollfd_ = epoll_create(1);
  if (epollfd_ == -1) {
    throw std::runtime_error("Failed to create epoll (" + std::string(strerror(errno)) + ")");
  }

  addSocketFd(serverSocketFd);
}

void Epoll::addSocketFd(int socketFd)
{
  mainEvent_.events = EPOLLIN;
  mainEvent_.data.fd = socketFd;

  if (epoll_ctl(epollfd_, EPOLL_CTL_ADD, socketFd, &mainEvent_) == -1) {
    throw std::runtime_error("Failed to add socket to epoll (" + std::string(strerror(errno)) + ")");
  }
}

void Epoll::removeSocketFd(int socketFd)
{
  mainEvent_.events = EPOLLIN;
  mainEvent_.data.fd = socketFd;
  epoll_ctl(epollfd_, EPOLL_CTL_DEL, socketFd, &mainEvent_);
}

int Epoll::getNextEventFd()
{
  if (eventQueue_.empty()) {
    struct epoll_event events[MAX_EVENTS];
    int nfds = epoll_wait(epollfd_, events, MAX_EVENTS, -1);
    if (nfds == -1) {
      throw std::runtime_error("Failed to get next event (" + std::string(strerror(errno)) + ")");
    }
    for (int i = 0; i < nfds; ++i) {
      eventQueue_.push(events[i]);
    }
  }

  nextEvent_ = eventQueue_.front();
  eventQueue_.pop();

  return nextEvent_.data.fd;
}
