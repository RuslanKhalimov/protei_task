#pragma once

#include <string>

class Socket
{
public:
  enum Type {
    TCP,
    UDP
  };

  Socket(int family, Type type, int protocol);
  ~Socket();
  void close();
  void bind(const std::string& addr, int port);
  void connect(const std::string& addr, int port);
  void listen(int n);
  Socket accept();
  void send(const std::string& msg);
  std::string recv();

private:
  explicit Socket(int sockfd);

  static const size_t MSG_BATCH_SIZE = 1024;
  int sockfd_ = -1;
  int family = -1;
};
