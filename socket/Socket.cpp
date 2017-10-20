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
#include <netinet/in.h>
#include <arpa/inet.h>

#define CHECK(x, m, handle) { if (x) == (m) { handle; return;}}
#define MAX_BUF_SIZE 1024

#include "../log/log.h"

namespace Rina {

void ServerSocket::init(SocketError *error) {
  LOG_INFO("Server Init")
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
  LOG_INFO("Stating the server...")

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

void ServerSocket::sendMessage(const Message &buf, const UserAddr &user, SocketError *error) {

}

void ServerSocket::recvMessage(Message &buf, const UserAddr &user, SocketError *error) {


}

void ServerSocket::stopServer(SocketError *error) {

}

void ClientSocket::init(SocketError *error) {
  // IP Address
  in_addr_t clientIP = inet_addr("127.0.0.1");
  int clientPort = 19845;

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  CHECK(sockfd, -1, {error = new SocketError(0, "Failed creating socket");});
  sockaddr_in clientSockAddr;
  clientSockAddr.sin_port = htons(clientPort);
  clientSockAddr.sin_family = AF_INET;
  clientSockAddr.sin_addr.s_addr = clientIP;
  int flag = bind(sockfd, (sockaddr* )&clientSockAddr, sizeof(sockaddr));
  CHECK(flag, -1, {error = new SocketError(0, "Failed binding socket");});
  UserAddr user(clientPort, clientSockAddr, sockfd);
  this->clientAddr = user;

  LOG_INFO("Client Socket init")

}

void ClientSocket::sendMessage(const Message &buf, SocketError *error) {
  if (clientAddr.sockfd != -1) {
    ssize_t sendFlag = send(clientAddr.sockfd, buf.content(), buf.size(), 0);
    LOG_INFO("Client Send Flag %u", sendFlag)
  } else {
    LOG_WARN("Socket Not Init")
  }
}

void ClientSocket::recvMessage(Message &buf, const UserAddr &user, SocketError *error) {
  ssize_t recvByteCount = 0;
  void* bufMem = malloc(sizeof(char)*MAX_BUF_SIZE);
  if (clientAddr.sockfd != -1) {
    recvByteCount = recv(clientAddr.sockfd, bufMem, MAX_BUF_SIZE, 0);
    
  }
}

void ClientSocket::stop(SocketError *error) {

}

void ClientSocket::conn(const UserAddr &serverAddr, SocketError *error) {
  sockaddr_in serverSockAddr = serverAddr.sockAddr;

  int flag = connect(this->clientAddr.sockfd, (sockaddr* )&serverSockAddr, sizeof(sockaddr));
  CHECK(flag, -1, {error = new SocketError(0, "Failed creating socket");});

}

}
