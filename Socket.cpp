#include "Socket.h"

#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>


int Socket::socket(int family, int type, int protocol)
{
  int sockfd = ::socket(family, type, protocol);
  if (sockfd == -1) {
    throw  std::runtime_error("Failed to create socket (" + std::string(strerror(errno)) + ")");
  }
  return sockfd;
}

void Socket::close(int sockfd)
{
  ::close(sockfd);
}

void Socket::fillSockaddr(const std::string& addr, int port, struct sockaddr *sockAddr, socklen_t *addrlen, int family)
{
  struct sockaddr_in sockaddrIn{};
  sockaddrIn.sin_family = family;
  sockaddrIn.sin_port = port;
  sockaddrIn.sin_addr.s_addr = inet_addr(addr.c_str());

  *(struct sockaddr_in*)sockAddr = sockaddrIn;
  *addrlen = static_cast<socklen_t>(sizeof(sockaddrIn));
}

void Socket::bind(int sockfd, const std::string& addr, int port, int family)
{
  struct sockaddr sockAddr{};
  socklen_t addrlen{};
  Socket::fillSockaddr(addr, port, &sockAddr, &addrlen, family);

  int status = ::bind(sockfd, &sockAddr, addrlen);
  if (status == -1) {
    throw std::runtime_error("Failed to bind socket at address : " + addr + ":" + std::to_string(port) + " (" + std::string(strerror(errno)) + ")");
  }
}

void Socket::connect(int sockfd, const std::string& addr, int port, int family)
{
  struct sockaddr sockAddr{};
  socklen_t addrlen{};
  Socket::fillSockaddr(addr, port, &sockAddr, &addrlen, family);

  int status = ::connect(sockfd, &sockAddr, addrlen);
  if (status == -1) {
    throw std::runtime_error("Failed to connect to address : " + addr + ":" + std::to_string(port) + " (" + std::string(strerror(errno)) + ")");
  }
}

void Socket::listen(int sockfd, int n)
{
  int status = ::listen(sockfd, n);
  if (status == -1) {
    throw std::runtime_error("Failed to listen (" + std::string(strerror(errno)) + ")");
  }
}

int Socket::accept(int sockfd)
{
  struct sockaddr_in clientAddr{};
  socklen_t len = sizeof(struct sockaddr_in);
  int clientFd = ::accept(sockfd, (struct sockaddr *) &clientAddr, &len);
  if (clientFd == -1) {
    throw std::runtime_error("Failed to accept (" + std::string(strerror(errno)) + ")");
  }

  char addr[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &(clientAddr.sin_addr), addr, INET_ADDRSTRLEN);
  std::cout << addr << ":" << clientAddr.sin_port << " connected" << std::endl;
  return clientFd;
}

void Socket::send(int sockfd, const std::string &msg)
{
  int status = ::send(sockfd, msg.c_str(), msg.length() + 1, 0);
  if (status == -1) {
    throw std::runtime_error("Failed to send message (" + std::string(strerror(errno)) + ")");
  }
}

std::string Socket::recv(int sockfd)
{
  std::string received;
  char msgBatch[MSG_BATCH_SIZE];
  while (true) {
    int bytesReceived = ::recv(sockfd, msgBatch, MSG_BATCH_SIZE, 0);
    if (bytesReceived == -1) {
      throw std::runtime_error("Failed to receive message (" + std::string(strerror(errno)) + ")");
    }
    if (bytesReceived > 0) {
      received.append(msgBatch);
    }
    if (bytesReceived < MSG_BATCH_SIZE) {
      return received;
    }
  }
}

void Socket::sendto(int sockfd, const std::string& msg, const struct sockaddr *dest_addr, socklen_t addrlen)
{
  int status = ::sendto(sockfd, msg.c_str(), msg.length() + 1, 0, dest_addr, addrlen);
  if (status == -1) {
    throw std::runtime_error("Failed to send message (" + std::string(strerror(errno)) + ")");
  }
}

std::string Socket::recvfrom(int sockfd, struct sockaddr *src_addr, socklen_t *addrlen)
{
  char buf[MSG_BATCH_SIZE];
  int status = ::recvfrom(sockfd, buf, MSG_BATCH_SIZE, 0, src_addr, addrlen);
  if (status == -1) {
    throw std::runtime_error("Failed to receive message (" + std::string(strerror(errno)) + ")");
  }
  return std::string(buf);
}

void Socket::setNonBlocking(int sockfd)
{
  int flags = fcntl(sockfd, F_GETFL, 0);
  if (flags == -1 || fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1) {
    throw std::runtime_error("Failed to make socket non-blocking (" + std::string(strerror(errno)) + ")");
  }
}
