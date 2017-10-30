/**
 * @brief Server Application
 * @author Vic Chan
 * @date 2017-10-23
 * */

#include "server.h"
#include "../log/log.h"
#include <string.h>


namespace Rina {

struct __handle {
  int fd;
  ServerSocket* serverSocket;
};

static void handle(void* handle) {

  __handle* clientHandle = (__handle* )handle;
  char buf[100];
  int sockfd = clientHandle->fd;
  while(true) {
    long len = clientHandle->serverSocket->recvMessage(sockfd, buf, 100);
    printf("%sRecv from [%d] Message: [%s]%s\n", CYAN, sockfd, buf, NONE);
    clientHandle->serverSocket->sendMessage(sockfd, buf, strlen(buf));
  }
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
    this->users[clientfd] = clientAddr;
    __handle* clientHandle = new __handle;
    clientHandle->fd = clientfd;
    clientHandle->serverSocket = this->serverSocket;
    this->threadManager.createThread(clientfd, handle, (void* )(clientHandle));

    LOG_INFO("%d join thr room", clientfd);
  }
  return 0;
}

}