#pragma once

#include "Socket.h"
#include <memory>
#include <vector>

class Server
{
public:
  explicit Server(Socket::Type type);
  void start(const std::string& addr, int port);

private:
  std::string handleMessage(const std::string& msg);
  void parseMessage(const std::string& msg, std::vector<uint64_t>& numbers);
  std::string generateResponse(std::vector<uint64_t>& numbers);

  static const size_t MAX_CONNECTIONS = 5;
  Socket socket_;
};
