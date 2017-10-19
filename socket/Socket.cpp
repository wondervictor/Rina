/**
 * Author: Vic Chan
 * Date: 2017/10/13
 *
 **/

#include "Socket.h"
#include <cstdio>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define CHECK(x, m, handle) { if (x) == (m) { handle; return;}}


#include "../log/log.h"

namespace Rina {

void ServerSocket::init(SocketError *error) {
  int port = 23333;
  sockaddr_in serverSockAddr;
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  CHECK(sockfd, -1, {error = new SocketError(0, "Failed creating socket");});

  serverSockAddr.sin_addr.s_addr = INADDR_ANY;
  serverSockAddr.sin_port=htons(port);
  serverSockAddr.sin_family = AF_INET;
  bzero((serverSockAddr.sin_zero), 8);

  UserAddr user(port, serverSockAddr, sockfd);
  this->serverAddr = user;

  int flag = bind(this->serverAddr.sockfd, (sockaddr* )&(this->serverAddr.sockAddr), sizeof(sockaddr));
  CHECK(flag, -1, {error = new SocketError(0, "Failed binding socket");});


}

void ServerSocket::startServer(SocketError *error) {
  LOG_INFO("Stating the server...");

  // Max Connecting
  int flag = listen(this->serverAddr.sockfd, 10);
  CHECK(flag, -1, {error = new SocketError(0, "Failed listening socket");});
  int sinSize = 0;
  sockaddr_in remoteAddr;
  while(1) {
    sinSize = sizeof(sockaddr_in);
    int client_fd = accept(this->serverAddr.sockfd, (sockaddr* )&remoteAddr, (socklen_t* )&sinSize);
    CHECK(client_fd, -1, {error = new SocketError(0, "Failed accepting socket");});


  }

}

void ServerSocket::stopServer(SocketError *error) {

}

void ClientSocket::init(SocketError *error) {

}

void ClientSocket::stop(SocketError *error) {

}

void ClientSocket::connect(const UserAddr &serverAddr, SocketError *error) {

}

}
