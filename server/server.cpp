/**
 * @brief Server Application
 * @author Vic Chan
 * @date 2017-10-23
 * */

#include "server.h"
#include "../log/log.h"
#include <string.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024


namespace Rina {

struct __handle {
  int fd;
  ServerSocket* serverSocket;
  RinaServer* server;
};

static void handle(void* handle) {

  __handle* clientHandle = (__handle* )handle;
  Message msgBuf;
  int sockfd = clientHandle->fd;
  LOG_INFO("Waiting for message")

  while(true) {
    long len = clientHandle->serverSocket->recvMessage(sockfd, &msgBuf, BUF_SIZE);
    if (len <= 0)
      continue;
    if (msgBuf.getType() == login) {
      std::string username = msgBuf.getUsername();
      sockaddr_in addr = clientHandle->serverSocket->getClients()[sockfd];
      std::string userIP = inet_ntoa(addr.sin_addr);
      //std::string userIP = msgBuf.getAddress();
      //long timestamp = msgBuf.getTime();
      User newUser = User();
      newUser.name = username;
      newUser.sockfd = sockfd;
      newUser.state = online;
      newUser.updateSeq = 0;
      clientHandle->server->login(newUser, userIP);
    }
  }


//  while(true) {
//    long len = clientHandle->serverSocket->recvMessage(sockfd, buf, 100);
//    if (len <= 0)
//      continue;
//    if (strcmp(buf, "logout") == 0) {
//      clientHandle->serverSocket->closeConn(sockfd);
//      printf("Current Online Users: ");
//      for(auto p: clientHandle->serverSocket->getClients()) {
//        printf("%d, ", p);
//      }
//      printf("\n");
//      LOG_INFO("[%d] logout", sockfd)
//      return;
//    }
//    printf("%sRecv from [%d] Message: [%s]%s\n", CYAN, sockfd, buf, NONE);
//    clientHandle->serverSocket->broadcast(buf,strlen(buf));
//    //printf("%sSend from [%d] Message: [%s]%s\n", CYAN, sockfd, buf, NONE);
//    //clientHandle->serverSocket->sendMessage(sockfd, buf, strlen(buf));
//  }
}

int RinaServer::init(int port) {
  LOG_INFO("Init Server")
  this->serverSocket = new ServerSocket();
  int flag = this->serverSocket->init(port);
  CHECK(flag, SOCKET_ERROR, LOG_WARN("Fail Init Server"))
  LOG_INFO("Finish Init Server")
  return 0;
}

int RinaServer::start() {
  SocketState state = this->serverSocket->getState();
  if (state == Error || state == Uninitialized) {
    LOG_ERROR("Server Socket Not Initialized or Error occurs")
    return -1;
  }
  this->serverSocket->startServer();
  LOG_INFO("Server Start")
  LOG_INFO("Server is %sRuning%s Now", PURPLE, NONE)
  int clientfd = 0;
  while(true) {
    sockaddr_in clientAddr;
    memset(&clientAddr, 0, sizeof(sockaddr_in));
    clientfd = this->serverSocket->acceptConn(&clientAddr);
    if (clientfd == SOCKET_ERROR) {
      LOG_WARN("Server cannot accept")
      continue;
    }
    this->userAddrs[clientfd] = clientAddr;
    __handle* clientHandle = new __handle;
    clientHandle->fd = clientfd;
    clientHandle->serverSocket = this->serverSocket;
    clientHandle->server = this;
    this->threadManager.createThread(clientfd, handle, (void* )(clientHandle));

    LOG_INFO("%d Connected the server", clientfd);
  }
  return 0;
}


int RinaServer::login(User user, std::string userIP) {
  this->users[user.sockfd] = user;
  LOG_INFO("User: %s from IP: %s joins the room now", user.name, userIP);
}

}