#pragma once

#include <string>
#include <memory>
#include <sys/epoll.h>

class Socket
{
public:
  enum Type {
    TCP,
    UDP
  };

  Socket() = default;
  explicit Socket(int sockfd);
  ~Socket();
  void init(int family, Type type, int protocol);
  int getFd();
  void bind(const std::string& addr, int port);
  void connect(const std::string& addr, int port);
  void listen(int n);
  std::shared_ptr<Socket> accept();
  void send(const std::string& msg);
  std::string recv();
  void setNonBlocking();

private:
  static const size_t MSG_BATCH_SIZE = 1024;
  int sockfd_ = -1;
  int family_ = -1;
};
