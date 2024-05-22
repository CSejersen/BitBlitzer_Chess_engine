#include "Socket_Handler.hpp"
#include <cstddef>
#include <vector>
#include <cstring>
#include <stdexcept>

void SocketHandler::connectToSocket(const std::string &socketPath) {
  if (sockfd != -1) {
    throw std::runtime_error("Already connected to a socket");
  }

  sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sockfd == -1) {
    throw std::runtime_error("Error creating socket");
  }

  struct sockaddr_un address {};
  address.sun_family = AF_UNIX;
  // Check if the socket path is longer than the buffer allows
  if (socketPath.size() >= sizeof(address.sun_path)) {
    throw std::runtime_error("Socket path too long");
  }
  std::copy(socketPath.begin(), socketPath.end(), address.sun_path);

  if (connect(sockfd, reinterpret_cast<struct sockaddr *>(&address),
              sizeof(address)) == -1) {
    close(sockfd);
    sockfd = -1;
    std::cout << "Socket path: " << socketPath << std::endl;
    throw std::runtime_error("Failed to connect");
  }
  std::cout << "Succesfully connected to socket" << std::endl;
}

std::string SocketHandler::readFromSocket() {
  size_t bufSize = 1024;
  std::vector<char> buf(bufSize);

  if (sockfd == -1) {
    throw std::runtime_error("Read error: No socket connection");
  }

  size_t n = read(sockfd, buf.data(), bufSize);
  if (n == -1) {
    throw std::runtime_error("Error reading from socket");
  }

  return std::string(buf.begin(), buf.begin() + n);
}

void SocketHandler::writeToSocket(const std::string &data) {
  if (sockfd == -1) {
    throw std::runtime_error("Write error: No socket connection");
  }

  ssize_t bytesWritten = write(sockfd, data.c_str(), data.size());
  if (bytesWritten == -1) {
    throw std::runtime_error("Error writing to socket");
  }
}
