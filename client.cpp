#include "Socket.h"
#include <sys/socket.h>
#include <iostream>
#include <getopt.h>


void printHelp(const char* programName)
{
  std::cout << "Usage:" << std::endl;
  std::cout << programName << " [options]" << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << "\t-h, --help   print this message and quit" << std::endl;
  std::cout << "\t-a, --addr   server address to connect to" << std::endl;
  std::cout << "\t-p, --port   server port to connect to" << std::endl;
  std::cout << "\t-t, --type   connection type (tcp, udp)" << std::endl;
}

int main(int argc, char *argv[])
{
  std::string addr;
  int port;
  Socket::Type type;

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
        std::string arg = std::string(optarg);
        if (arg == "tcp") {
          type = Socket::TCP;
        }
        else if (arg == "udp") {
          type = Socket::UDP;
        }
        else {
          std::cout << "Unknown type parameter : " << type << std::endl;
          std::cout << "Supported type : tcp, udp" << std::endl;
          exit(1);
        }
        break;
    }
  }

  Socket socket;
  try {
    socket.init(AF_INET, type, 0);
    socket.connect(addr, port);
    std::string msg;
    while (true) {
      std::cin >> msg;
      socket.send(msg);
      if (msg == "stop") {
        break;
      }
      std::cout << socket.recv() << std::endl;
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}
