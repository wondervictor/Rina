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

#define SOCKET_ERROR  (-1)

#define CHECK(x, m, handle) if ((x) == (m)) { \
                              handle; \
                              return -1; \
                            }
#define MAX_BUF_SIZE 1024

#include "../log/log.h"

namespace Rina {

// server socket init
int ServerSocket::init(int port) {
  LOG_INFO("Server Init")
  sockaddr_in serverSockAddr;
  memset(&serverSockAddr, 0, sizeof(serverSockAddr));
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  CHECK(sockfd, SOCKET_ERROR, LOG_ERROR("Server Socket Init Failed"))
  serverSockAddr.sin_addr.s_addr = inet_addr("0.0.0.0");
  serverSockAddr.sin_port=htons(port);
  serverSockAddr.sin_family = AF_INET;
//  bzero((serverSockAddr.sin_zero), 8);
  this->port = htons(port);
  this->sockfd = sockfd;
  this->serverAddr = serverSockAddr;
  int flag = bind(sockfd, (sockaddr* )&serverSockAddr, sizeof(serverSockAddr));
  CHECK(flag, SOCKET_ERROR, LOG_ERROR("Server Socket Bind Failed"))
  LOG_INFO("Server Socket Init Success")
  return sockfd;
}

// start server
int ServerSocket::startServer() {
  LOG_INFO("Stating The Server...")
  // Max Connecting
  int flag = listen(this->sockfd, this->maxConn);

  printf("flag:%d\n",flag);

  //printf("this->sockfd=%d--this->maxConn=%d\n",this->sockfd, this->maxConn);

  CHECK(flag, SOCKET_ERROR, LOG_ERROR("Server Socket Listen Failed"));
  return 1;
}

// send message
int ServerSocket::sendMessage(int sockfd, void *buf, size_t size) {
  ssize_t sendSize = send(sockfd, buf, size, 0);
  CHECK(sendSize, SOCKET_ERROR, LOG_ERROR("Server Socket Send Error"))
  return 1;
}

long ServerSocket::recvMessage(int sockfd, void *buf, size_t size) {
  //
  ssize_t recvSize = recv(sockfd, buf, size, 0);
  CHECK(recvSize, SOCKET_ERROR, LOG_ERROR("Server Socket Recv Error"))
  return recvSize;
}

int ServerSocket::acceptConn(sockaddr_in *clientAddr) {
  int sinsize = sizeof(struct sockaddr_in);
  int clientfd = accept(this->sockfd, (sockaddr* )&clientAddr, (socklen_t* )&sinsize);
  //
  printf("clientfd:%d\n",clientfd);
  CHECK(clientfd, SOCKET_ERROR, LOG_ERROR("Server Accept Error"))
  //this->clients.push_back(clientfd);
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
  return 1;
}

std::vector<int> ServerSocket::getClients() {
  return this->clients;
}


int ClientSocket::init(int port) {

  in_addr_t clientIP = inet_addr("0.0.0.0");
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  CHECK(sockfd, -1, LOG_ERROR("Client Socket Init Failed"))

  sockaddr_in clientSockAddr;
  memset(&clientSockAddr, 0, sizeof(clientSockAddr));

  clientSockAddr.sin_port = htons(port);
  clientSockAddr.sin_family = AF_INET;
  clientSockAddr.sin_addr.s_addr = clientIP;

  int flag = bind(sockfd, (sockaddr* )&clientSockAddr, sizeof(clientSockAddr));
  CHECK(flag, SOCKET_ERROR, LOG_ERROR("Client Socket Bind Error"))
  printf("%d\n",errno);


  this->sockfd = sockfd;
  this->port = port;
  this->clientAddr = clientSockAddr;

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

  //EINVAL 22
//The address_len argument is not a valid length for the address family;
//or invalid address family in the sockaddr structure.

  printf("%d\n",errno);

  CHECK(flag, SOCKET_ERROR, LOG_ERROR("Client Connect %s Failed", serverIPStr.c_str()))
  LOG_INFO("Client Connect %s Successed", serverIPStr.c_str())
  this->connectAddr = server;
  return 1;
}



int ClientSocket::sendMessage(void *buf, size_t size) {
  ssize_t sendSize = send(sockfd, buf, size, 0);
  CHECK(sendSize, SOCKET_ERROR, LOG_ERROR("Client Socket Send Error"))
  return 1;
}

long ClientSocket::recvMessage(void *buf, size_t size) {
  ssize_t recvSize = recv(sockfd, buf, size, 0);
  CHECK(recvSize, SOCKET_ERROR, LOG_ERROR("Client Socket Recv Error"))
  return recvSize;
}

int ClientSocket::stop() {
  close(this->sockfd);
  LOG_INFO("Socket Close")
  return 1;
}


}
