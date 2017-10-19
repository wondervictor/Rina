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


namespace Rina {

void ServerSocket::init(SocketError *error) {
  int port = 23333;
  sockaddr_in serverSockAddr;
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    error = new SocketError(0, "Failed creating socket");
    return;
  }

  serverSockAddr.sin_addr.s_addr = INADDR_ANY;
  serverSockAddr.sin_port=htons(port);
  serverSockAddr.sin_family = AF_INET;
  bzero((serverSockAddr.sin_zero), 8);

  UserAddr user(port, serverSockAddr, sockfd);

  this->serverAddr = user;


}

void ServerSocket::startServer(SocketError *error) {


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
