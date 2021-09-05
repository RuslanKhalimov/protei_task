#include "Server.h"
#include <sys/socket.h>
#include <sstream>
#include <algorithm>
#include <iostream>


void Server::initSocket(const std::string &addr, int port, Socket::Type type)
{
  socket_.init(AF_INET, type, 0);
  socket_.bind(addr, port);
  socket_.listen(MAX_CONNECTIONS);
}

void Server::start(const std::string &addr, int port, Socket::Type type)
{
  initSocket(addr, port, type);

  while (true) {
    std::shared_ptr<Socket> clientSocket = socket_.accept();
    while (true) {
      std::string msg = clientSocket->recv();
      std::cout << msg << std::endl;
      if (msg == "stop" || msg.empty()) {
        break;
      }
      clientSocket->send(handleMessage(msg));
    }
  }
}

std::string Server::handleMessage(const std::string &msg)
{
  std::vector<uint64_t> numbers;
  parseMessage(msg, numbers);
  return generateResponse(numbers);
}

void Server::parseMessage(const std::string &msg, std::vector<uint64_t> &numbers)
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

std::string Server::generateResponse(std::vector<uint64_t> &numbers)
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

void AsyncServer::start(const std::string &addr, int port, Socket::Type type)
{
  initSocket(addr, port, type);
  socket_.setNonBlocking();
  epoll_.init(socket_.getFd());

  while (true) {
    int eventFd = epoll_.getNextEventFd();
    if (eventFd == socket_.getFd()) { // new connection
      std::shared_ptr<Socket> clientSocket = socket_.accept();
      clientSocket->setNonBlocking();
      epoll_.addSocketFd(clientSocket->getFd());
      clients_.insert({clientSocket->getFd(), clientSocket});
    }
    else { // message
      std::shared_ptr<Socket> clientSocket = clients_.at(eventFd);
      std::string msg = clientSocket->recv();
      std::cout << msg << std::endl;
      if (msg == "stop" || msg.empty()) {
        epoll_.removeSocketFd(eventFd);
      }
      else {
        clientSocket->send(handleMessage(msg));
      }
    }
  }
}
