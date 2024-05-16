#include <cstring>
#include <iostream>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

class SocketHandler {
public:
  SocketHandler() : sockfd(-1) {}

  ~SocketHandler() {
    if (sockfd != -1) {
      close(sockfd);
    }
  }

  void connectToSocket(const std::string &socketPath);
  std::string readFromSocket();
  void writeToSocket(const std::string &data); 

private:
  int sockfd;
};

