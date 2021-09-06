#pragma once

#include "Socket.h"


class Client
{
public:
  virtual ~Client();
  void start(const std::string& addr, int port);

protected:
  virtual void initSocket(const std::string& addr, int port) = 0;
  virtual void send(const std::string& msg) = 0;
  virtual std::string recv() = 0;

  int sockfd_ = -1;
};

class TcpClient : public Client
{
protected:
  void initSocket(const std::string& addr, int port) override;
  void send(const std::string& msg) override;
  std::string recv() override;
};

class UdpClient : public Client
{
protected:
  void initSocket(const std::string& addr, int port) override;
  void send(const std::string& msg) override;
  std::string recv() override;

  struct sockaddr serverAddr_{};
  socklen_t addrlen_{};
};
