#pragma once

#include <string>
#include <memory>
#include <sys/socket.h>
#include <sys/epoll.h>

namespace Socket
{
  int socket(int family, int type, int protocol);
  void close(int sockfd);
  void fillSockaddr(const std::string& addr, int port, struct sockaddr *sockaddr, socklen_t *addrlen, int family = AF_INET);
  void bind(int sockfd, const std::string& addr, int port, int family = AF_INET);
  void connect(int sockfd, const std::string& addr, int port, int family = AF_INET);
  void listen(int sockfd, int n);
  int accept(int sockfd);
  void send(int sockfd, const std::string& msg);
  std::string recv(int sockfd);
  void sendto(int sockfd, const std::string& msg, const struct sockaddr *dest_addr, socklen_t addrlen);
  std::string recvfrom(int sockfd, struct sockaddr *src_addr, socklen_t *addrlen);
  void setNonBlocking(int sockfd);

  static const size_t MSG_BATCH_SIZE = 1024;
};
