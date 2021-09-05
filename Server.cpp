#include "Server.h"
#include <sys/socket.h>
#include <sstream>
#include <algorithm>
#include <iostream>


Server::Server(Socket::Type type)
    : socket_(Socket(AF_INET, type, 0))
{
}

void Server::start(const std::string &addr, int port)
{
  socket_.bind(addr, port);
  socket_.listen(MAX_CONNECTIONS);

  while (true) {
    Socket clientSocket = socket_.accept();
    while (true) {
      std::string msg = clientSocket.recv();
      std::cout << msg << std::endl;
      if (msg == "stop" || msg.empty()) {
        break;
      }
      clientSocket.send(handleMessage(msg));
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
