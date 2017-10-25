/**
 * @brief Socket
 * @author Vic Chan
 * @date 2017-10-23
 * */


#include "Socket.h"
#include <cstdio>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>

#define MAX_BUF_SIZE 1024

#include "../log/log.h"

namespace Rina {

// server socket init
int ServerSocket::init(int port) {
  LOG_INFO("Server Init")
  sockaddr_in serverSockAddr;
  memset(&serverSockAddr, 0, sizeof(serverSockAddr));
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  CHECK(sockfd, SOCKET_ERROR, {LOG_ERROR("Server Socket Init Failed") state=Error;})
  serverSockAddr.sin_addr.s_addr = inet_addr("0.0.0.0");
  serverSockAddr.sin_port=htons(port);
  serverSockAddr.sin_family = AF_INET;
  this->port = htons(port);
  this->sockfd = sockfd;
  this->serverAddr = serverSockAddr;
  int flag = bind(sockfd, (sockaddr* )&serverSockAddr, sizeof(serverSockAddr));
  CHECK(flag, SOCKET_ERROR, {LOG_ERROR("Server Socket Bind Failed") state=Error; })
  LOG_INFO("Server Socket Init Success")
  state = Stopped;
  return sockfd;
}

// start server
int ServerSocket::startServer() {
  LOG_INFO("Stating The Server...")
  // Max Connecting
  int flag = listen(this->sockfd, this->maxConn);

  printf("flag:%d\n",flag);

  CHECK(flag, SOCKET_ERROR, {LOG_ERROR("Server Socket Listen Failed") state=Error;});

  state = Running;

  return 1;
}

// send message
int ServerSocket::sendMessage(int sockfd, void *buf, size_t size) {
  ssize_t sendSize = send(sockfd, buf, size, 0);
  CHECK(sendSize, SOCKET_ERROR, {LOG_ERROR("Server Socket Send Error") state=Error;})
  return 1;
}

long ServerSocket::recvMessage(int sockfd, void *buf, size_t size) {
  //
  ssize_t recvSize = recv(sockfd, buf, size, 0);
  CHECK(recvSize, SOCKET_ERROR, {LOG_ERROR("Server Socket Recv Error") state=Error;})
  return recvSize;
}

int ServerSocket::acceptConn(sockaddr_in *clientAddr) {
  int sinsize = sizeof(struct sockaddr_in);
  int clientfd = accept(this->sockfd, (sockaddr* )&clientAddr, (socklen_t* )&sinsize);
  CHECK(clientfd, SOCKET_ERROR, {LOG_ERROR("Server Accept Error") state=Error;})
  LOG_INFO("Server Add Client: %d", clientfd)
  return clientfd;
}

int ServerSocket::broadcast(void *buf, size_t size) {
  int sum = 0;
  for(auto clientfd: this->clients) {
    int flag = sendMessage(clientfd, buf, size);
    if (flag == 1) {
      sum ++;
    } else {
      LOG_WARN("Server Send: %d Failed", clientfd);
    }
  }
  return sum;
}

int ServerSocket::stopServer() {
  LOG_INFO("Server Stop")
  close(this->sockfd);
  state=Stopped;
  return 1;
}

std::vector<int> ServerSocket::getClients() {
  return this->clients;
}


int ClientSocket::init(int port) {

  in_addr_t clientIP = inet_addr("0.0.0.0");
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  CHECK(sockfd, -1, {LOG_ERROR("Client Socket Init Failed") state=Error;})

  sockaddr_in clientSockAddr;
  memset(&clientSockAddr, 0, sizeof(clientSockAddr));

  clientSockAddr.sin_port = htons(port);
  clientSockAddr.sin_family = AF_INET;
  clientSockAddr.sin_addr.s_addr = clientIP;

  int flag = bind(sockfd, (sockaddr* )&clientSockAddr, sizeof(clientSockAddr));
  CHECK(flag, SOCKET_ERROR, {LOG_ERROR("Client Socket Bind Error") state=Error;})

  this->sockfd = sockfd;
  this->port = port;
  this->clientAddr = clientSockAddr;
  state=Running;
  LOG_INFO("Client Socket Init Success")
  return 1;
}

int ClientSocket::conn(const std::string& serverIPStr, int port) {
  in_addr_t serverIP = inet_addr(serverIPStr.c_str());

  sockaddr_in server;
  memset(&server, 0, sizeof(server));

  server.sin_addr.s_addr = serverIP;
  server.sin_family = AF_INET;
  server.sin_port = htons(port);

  int flag = connect(this->sockfd,(const struct sockaddr *)&server,(socklen_t)sizeof(server));

  CHECK(flag, SOCKET_ERROR, {LOG_ERROR("Client Connect %s Failed", serverIPStr.c_str()) state=Error;})
  LOG_INFO("Client Connect %s Successed", serverIPStr.c_str())
  this->connectAddr = server;
  return 1;
}



int ClientSocket::sendMessage(void *buf, size_t size) {
  ssize_t sendSize = send(sockfd, buf, size, 0);
  CHECK(sendSize, SOCKET_ERROR, {LOG_ERROR("Client Socket Send Error") state=Error;})
  return 1;
}

long ClientSocket::recvMessage(void *buf, size_t size) {
  ssize_t recvSize = recv(sockfd, buf, size, 0);
  CHECK(recvSize, SOCKET_ERROR, {LOG_ERROR("Client Socket Recv Error") state=Error;})
  return recvSize;
}

int ClientSocket::stop() {
  close(this->sockfd);
  LOG_INFO("Socket Close")
  state=Stopped;
  return 1;
}


}
