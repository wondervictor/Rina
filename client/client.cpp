/**
 * @brief Client Application
 * @author Vic Chan
 * @date 2017-10-23
 * */


#include "client.h"
#include "../log/log.h"
#include <string.h>

namespace Rina {

static void handleSendMessage(void* clientSocket) {
  char buf[100];
  ClientSocket* pclientSocket = (ClientSocket* )clientSocket;
  while(true) {
    printf(">>>: ");
    scanf("%s", buf);
    size_t len = strlen(buf);
    int flag = pclientSocket->sendMessage(buf, len);
    if (flag == SOCKET_ERROR) {
      LOG_WARN("Send Server Failed: %s",buf)
      continue;
    } else {
      printf("Send to Server: %s\n", buf);
    }
  }
}

static void handleRecvMesage(void* clientSocket) {
  char buf[100];
  ClientSocket* pclientSocket = (ClientSocket* )clientSocket;
  while(true) {
    ssize_t len =pclientSocket->recvMessage(buf, 100);
    if (len == 0)
      continue;
    if (len == SOCKET_ERROR) {
      LOG_WARN("Receive Server Failed: %s",buf)
      continue;
    } else {
      printf("Received from server: %s\n", buf);
    }
  }
}

int RinaClient::init(int port) {
  LOG_INFO("Init Client")
  this->clientSocket = new ClientSocket();
  int flag = this->clientSocket->init(port);
  CHECK(flag, SOCKET_ERROR, LOG_WARN("Fail Init Client Socket"))
  LOG_INFO("Finish Init Server")
  return 0;
}

int RinaClient::login(std::string &server, int port) {
  LOG_INFO("Client Login!")
  int flag = this->clientSocket->conn(server, port);
  CHECK(flag, SOCKET_ERROR, LOG_WARN("Fail Connect Server"))
  LOG_INFO("Login Success")
  char buf[100];
  //ClientSocket* pclientSocket = (ClientSocket* )clientSocket;
  while(true) {
    printf(">>>: ");
    scanf("%s", buf);
    if (strcmp(buf, "logout") == 0) {
      LOG_INFO("Preparing to log out")
      logout();
      break;
    }
    size_t len = strlen(buf);
    int flag = this->clientSocket->sendMessage(buf, len);
    if (flag == SOCKET_ERROR) {
      LOG_WARN("Send Server Failed: %s",buf)
      continue;
    } else {
      printf("Send to Server: %s\n", buf);
    }

    len = this->clientSocket->recvMessage(buf, 100);
    if (len == 0)
      continue;
    if (len == SOCKET_ERROR) {
      LOG_WARN("Receive Server Failed: %s",buf)
      continue;
    } else {
      printf("Received from server: %s\n", buf);
    }
  }
  //manager->createThread(0, handleSendMessage, (void* )&this->clientSocket);
  //manager->createThread(1, handleRecvMesage, (void* )&this->clientSocket);
  //while(true)
  //  DEBUG("Run")
  return 0;
}

int RinaClient::logout() {
  LOG_INFO("Client Stops")
  std::string username = this->user->name;

  Message message(username, LOGIN, "", );
  this->clientSocket->sendMessage(message, strlen(message));
  this->clientSocket->stop();
  return 0;
}

}