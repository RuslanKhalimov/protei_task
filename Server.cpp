#include "Server.h"
#include <sys/socket.h>
#include <sstream>
#include <algorithm>
#include <iostream>


Server::~Server()
{
  if (sockfd_ != -1) {
    Socket::close(sockfd_);
  }
}

std::string Server::handleMessage(const std::string& msg)
{
  std::vector<uint64_t> numbers;
  parseMessage(msg, numbers);
  return generateResponse(numbers);
}

void Server::parseMessage(const std::string& msg, std::vector<uint64_t>& numbers)
{
  uint64_t nextNum = 0;
  bool foundNumber = false;
  for (char c : msg) {
    if (isdigit(c)) {
      nextNum = nextNum * 10 + (c - '0');
      foundNumber = true;
    } else if (foundNumber) {
      numbers.push_back(nextNum);
      nextNum = 0;
      foundNumber = false;
    }
  }
  if (foundNumber) {
    numbers.push_back(nextNum);
  }
}

std::string Server::generateResponse(std::vector<uint64_t>& numbers)
{
  std::stringstream response;

  std::sort(numbers.begin(), numbers.end());
  uint64_t sum = 0;
  for (int i = 0; i < numbers.size(); ++i) {
    sum += numbers[i];
    if (i > 0) {
      response << " ";
    }
    response << numbers[i];
  }
  response << std::endl << sum;

  return response.str();
}

void TcpServer::start(const std::string& addr, int port)
{
  initSocket(addr, port);
  Socket::setNonBlocking(sockfd_);
  epoll_.init(sockfd_);

  while (true) {
    int eventFd = epoll_.getNextEventFd();
    if (eventFd == sockfd_) { // new connection
      int clientFd = Socket::accept(sockfd_);
      Socket::setNonBlocking(clientFd);
      epoll_.addSocketFd(clientFd);
    }
    else { // message
      int clientFd = eventFd;
      std::string msg = Socket::recv(clientFd);
      std::cout << msg << std::endl;
      if (msg == "stop" || msg.empty()) {
        epoll_.removeSocketFd(clientFd);
        Socket::close(clientFd);
      }
      else {
        Socket::send(clientFd, handleMessage(msg));
      }
    }
  }
}

void TcpServer::initSocket(const std::string& addr, int port)
{
  sockfd_ = Socket::socket(AF_INET, SOCK_STREAM, 0);
  Socket::bind(sockfd_, addr, port);
  Socket::listen(sockfd_, MAX_CONNECTIONS);
}

void UdpServer::start(const std::string &addr, int port)
{
  initSocket(addr, port);

  struct sockaddr clientAddr{};
  socklen_t addrlen = sizeof(sockaddr);

  while (true) {
    std::string msg = Socket::recvfrom(sockfd_, &clientAddr, &addrlen);
    std::cout << msg << std::endl;
    if (msg != "stop" && !msg.empty()) {
      Socket::sendto(sockfd_, handleMessage(msg), &clientAddr, addrlen);
    }
  }
}

void UdpServer::initSocket(const std::string& addr, int port)
{
  sockfd_ = Socket::socket(AF_INET, SOCK_DGRAM, 0);
  Socket::bind(sockfd_, addr, port);
}
