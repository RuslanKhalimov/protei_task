#pragma once

#include "Socket.h"


class Client
{
public:
  virtual ~Client();
  virtual void start(const std::string& addr, int port) = 0;

protected:
  int sockfd_ = -1;
};

class TcpClient : public Client
{
public:
  void start(const std::string& addr, int port) override;

private:
  void initSocket(const std::string& addr, int port);
};

class UdpClient : public Client
{
public:
  void start(const std::string& addr, int port) override;

private:
  void initSocket(const std::string& addr, int port);

  struct sockaddr serverAddr_{};
  socklen_t addrlen_{};
};
