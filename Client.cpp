#include <iostream>
#include "Client.h"


Client::~Client()
{
  if (sockfd_ != -1) {
    Socket::close(sockfd_);
  }
}

void TcpClient::start(const std::string &addr, int port)
{
  initSocket(addr, port);

  std::string msg;
  while (true) {
    std::cin >> msg;
    Socket::send(sockfd_, msg);
    if (msg == "stop") {
      break;
    }
    std::cout << Socket::recv(sockfd_) << std::endl;
  }
}

void TcpClient::initSocket(const std::string &addr, int port)
{
  sockfd_ = Socket::socket(AF_INET, SOCK_STREAM, 0);
  Socket::connect(sockfd_, addr, port);
}

void UdpClient::start(const std::string &addr, int port)
{
  initSocket(addr, port);

  std::string msg;
  while (true) {
    std::cin >> msg;
    Socket::sendto(sockfd_, msg, &serverAddr_, addrlen_);
    if (msg == "stop") {
      break;
    }
    std::cout << Socket::recvfrom(sockfd_, &serverAddr_, &addrlen_) << std::endl;
  }
}

void UdpClient::initSocket(const std::string &addr, int port)
{
  sockfd_ = Socket::socket(AF_INET, SOCK_DGRAM, 0);
  Socket::fillSockaddr(addr, port, &serverAddr_, &addrlen_);
}
