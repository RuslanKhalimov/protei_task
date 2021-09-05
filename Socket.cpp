#include "Socket.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <fcntl.h>


Socket::Socket(int sockfd)
    : sockfd_(sockfd)
{
}

Socket::~Socket()
{
  if (sockfd_ != -1) {
    ::close(sockfd_);
  }
}

void Socket::init(int family, Socket::Type type, int protocol)
{
  family_ = family;
  if (type == TCP) {
    sockfd_ = socket(family, SOCK_STREAM, protocol);
  }
  else { // UDP
    sockfd_ = socket(family, SOCK_DGRAM, protocol);
  }
  if (sockfd_ == -1) {
    throw  std::runtime_error("Failed to create socket (" + std::string(strerror(errno)) + ")");
  }
}

int Socket::getFd()
{
  return sockfd_;
}

void Socket::bind(const std::string &addr, int port)
{
  struct sockaddr_in sockaddrIn{};
  sockaddrIn.sin_family = family_;
  sockaddrIn.sin_port = port;
  sockaddrIn.sin_addr.s_addr = inet_addr(addr.c_str());
  int status = ::bind(sockfd_, (struct sockaddr *) &sockaddrIn, sizeof(sockaddrIn));
  if (status == -1) {
    throw std::runtime_error("Failed to bind socket at address : " + addr + ":" + std::to_string(port) + " (" + std::string(strerror(errno)) + ")");
  }
}

void Socket::connect(const std::string &addr, int port)
{
  struct sockaddr_in sockaddrIn{};
  sockaddrIn.sin_family = family_;
  sockaddrIn.sin_port = port;
  sockaddrIn.sin_addr.s_addr = inet_addr(addr.c_str());
  int status = ::connect(sockfd_, (struct sockaddr *) &sockaddrIn, sizeof(sockaddrIn));
  if (status == -1) {
    throw std::runtime_error("Failed to connect to address : " + addr + ":" + std::to_string(port) + " (" + std::string(strerror(errno)) + ")");
  }
}

void Socket::listen(int n)
{
  int status = ::listen(sockfd_, n);
  if (status == -1) {
    throw std::runtime_error("Failed to listen (" + std::string(strerror(errno)) + ")");
  }
}

std::shared_ptr<Socket> Socket::accept()
{
  struct sockaddr_in clientAddr{};
  socklen_t len = sizeof(struct sockaddr_in);
  int clientFd = ::accept(sockfd_, (struct sockaddr *) &clientAddr, &len);
  if (clientFd == -1) {
    throw std::runtime_error("Failed to accept (" + std::string(strerror(errno)) + ")");
  }

  char addr[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &(clientAddr.sin_addr), addr, INET_ADDRSTRLEN);
  std::cout << addr << ":" << clientAddr.sin_port << " connected" << std::endl;
  return std::make_shared<Socket>(clientFd);
}

void Socket::send(const std::string &msg)
{
  int status = ::send(sockfd_, msg.c_str(), msg.length() + 1, 0);
  if (status == -1) {
    throw std::runtime_error("Failed to send message (" + std::string(strerror(errno)) + ")");
  }
}

std::string Socket::recv()
{
  std::string received;
  char msgBatch[MSG_BATCH_SIZE];
  while (true) {
    int bytesReceived = ::recv(sockfd_, msgBatch, MSG_BATCH_SIZE, 0);
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

void Socket::setNonBlocking()
{
  int flags = fcntl(sockfd_, F_GETFL, 0);
  if (flags == -1 || fcntl(sockfd_, F_SETFL, flags | O_NONBLOCK) == -1) {
    throw std::runtime_error("Failed to make socket non-blocking (" + std::string(strerror(errno)) + ")");
  }
}
