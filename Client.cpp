#include <iostream>
#include "Client.h"


Client::~Client()
{
  if (sockfd_ != -1) {
    Socket::close(sockfd_);
  }
}

void Client::start(const std::string &addr, int port)
{
  initSocket(addr, port);

  std::string msg;
  while (true) {
    getline(std::cin, msg);
    send(msg);
    if (msg == "stop") {
      break;
    }
    std::cout << recv() << std::endl;
  }
}

void TcpClient::initSocket(const std::string &addr, int port)
{
  sockfd_ = Socket::socket(AF_INET, SOCK_STREAM, 0);
  Socket::connect(sockfd_, addr, port);
}

void TcpClient::send(const std::string &msg)
{
  Socket::send(sockfd_, msg);
}

std::string TcpClient::recv()
{
  return Socket::recv(sockfd_);
}

void UdpClient::initSocket(const std::string &addr, int port)
{
  sockfd_ = Socket::socket(AF_INET, SOCK_DGRAM, 0);
  Socket::fillSockaddr(addr, port, &serverAddr_, &addrlen_);
}

void UdpClient::send(const std::string &msg)
{
  Socket::sendto(sockfd_, msg, &serverAddr_, addrlen_);
}

std::string UdpClient::recv()
{
  return Socket::recvfrom(sockfd_, &serverAddr_, &addrlen_);
}
