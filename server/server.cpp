#include "Server.h"
#include <iostream>
#include <getopt.h>

void printHelp(const char* programName)
{
  std::cout << "Usage:" << std::endl;
  std::cout << programName << " [options]" << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << "\t-h, --help   print this message and quit" << std::endl;
  std::cout << "\t-a, --addr   server address" << std::endl;
  std::cout << "\t-p, --port   server port" << std::endl;
  std::cout << "\t-t, --type   connection type (tcp, udp)" << std::endl;
}

int main(int argc, char *argv[])
{
  std::string addr;
  int port;
  std::string type;

  static struct option longOptions[] = {
      {"help", 0, 0, 'h'},
      {"addr", 1, 0, 'a'},
      {"port", 1, 0, 'p'},
      {"type", 1, 0, 't'},
      {0, 0, 0, 0}
  };

  int optionIndex = 0;
  while (true) {
    int c = getopt_long(argc, argv, "a:p:t:", longOptions, &optionIndex);
    if (c == -1) {
      break;
    }

    switch (c) {
      case 'h':
        printHelp(argv[0]);
        break;
      case 'a':
        addr = std::string(optarg);
        break;
      case 'p':
        port = atoi(optarg);
        if (port == 0) {
          std::cout << "Invalid port : " << optarg << std::endl;
          exit(1);
        }
        break;
      case 't':
        type = std::string(optarg);
        if (type != "tcp" && type != "udp") {
          std::cout << "Unknown type parameter : " << type << std::endl;
          std::cout << "Supported type : tcp, udp" << std::endl;
          exit(1);
        }
        break;
    }
  }

  std::unique_ptr<Server> server = nullptr;
  if (type == "tcp") {
    server = std::make_unique<TcpServer>();
  }
  else {
    server = std::make_unique<UdpServer>();
  }
  try {
    server->start(addr, port);
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}
