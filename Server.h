#pragma once

#include "Socket.h"
#include "Epoll.h"
#include <memory>
#include <map>

class Server
{
public:
  virtual ~Server();
  virtual void start(const std::string& addr, int port) = 0;

protected:
  std::string handleMessage(const std::string& msg);

  int sockfd_;

private:
  void parseMessage(const std::string& msg, std::vector<uint64_t>& numbers);
  std::string generateResponse(std::vector<uint64_t>& numbers);
};

class TcpServer : public Server
{
public:
  void start(const std::string& addr, int port) override;

private:
  void initSocket(const std::string& addr, int port);

  static const size_t MAX_CONNECTIONS = 5;
  Epoll epoll_;
};

class UdpServer : public Server
{
public:
  void start(const std::string& addr, int port) override;

private:
  void initSocket(const std::string& addr, int port);
};
