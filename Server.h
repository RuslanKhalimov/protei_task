#pragma once

#include "Socket.h"
#include "Epoll.h"
#include <memory>
#include <map>

class Server
{
public:
  virtual void start(const std::string& addr, int port, Socket::Type type);

protected:
  void initSocket(const std::string &addr, int port, Socket::Type type);
  std::string handleMessage(const std::string& msg);

  static const size_t MAX_CONNECTIONS = 5;
  Socket socket_;

private:
  void parseMessage(const std::string& msg, std::vector<uint64_t>& numbers);
  std::string generateResponse(std::vector<uint64_t>& numbers);
};

class AsyncServer : public Server
{
public:
  void start(const std::string& addr, int port, Socket::Type type) override;

private:
  Epoll epoll_;
  std::map<int, std::shared_ptr<Socket>> clients_;
};
