/**
 * @brief Client Application
 * @author Vic Chan
 * @date 2017-10-23
 * */


#include "client.h"
#include "../log/log.h"
#include <string.h>
#include <iostream>

namespace Rina {

std::string DefaultIP = "0.0.0.0";

static void handleSendMessage(void* clientSocket) {
  char buf[100];
  ClientSocket* pclientSocket = (ClientSocket* )clientSocket;
  while(true) {
    printf(">>>: ");
    scanf("%s", buf);
    if (strcmp(buf, "login") == 0) {

    }
    size_t len = strlen(buf);
    int flag = pclientSocket->sendMessage(buf, len);
    if (flag == SOCKET_ERROR) {
      LOG_WARN("Send Server Failed: %s",buf)
      continue;
    } else {
      printf("Send to Server: %s\n", buf);
    }
    break;
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
  char buf[1024];
  //ClientSocket* pclientSocket = (ClientSocket* )clientSocket;
  while(true) {
    printf(">>>: ");
    scanf("%s", buf);
    int flag = 0;
    size_t len;
    if (strcmp(buf, "login") == 0) {
      LOG_INFO("Preparing to log in")
      std::string sendbuf = LOGIN;
      long time = getTime();
      Message message = Message(user->name, sendbuf, DefaultIP, time);
      flag = this->clientSocket->sendMessage(&message, sizeof(message));
    }
    else if (strcmp(buf, "logout") == 0) {
      LOG_INFO("Preparing to log out")
      std::string sendbuf = LOGOUT;
      long time = getTime();
      Message message = Message(user->name, sendbuf, DefaultIP, time);
      this->clientSocket->sendMessage(&message, sizeof(message));
      logout();
      break;
    } else if (strcmp(buf, "getall") == 0){
      LOG_INFO("Request for all records")
      std::string sendbuf = GET_ALL;
      long time = getTime();
      Message message = Message(user->name, sendbuf, DefaultIP, time);
      flag = this->clientSocket->sendMessage(&message, sizeof(message));

      if (flag == SOCKET_ERROR) {
        LOG_WARN("Send Server Failed: %s",GET_ALL)
        continue;
      } else {
        printf("Send to Server: %s\n", GET_ALL);
      }
      auto* messages = new MultiMessage();
      ssize_t recvLen = this->clientSocket->recvMessage(messages, 1024);
      if (recvLen == 0)
        continue;
      if (recvLen == SOCKET_ERROR) {
        LOG_WARN("Receive Server Failed: %s",buf)
        continue;
      } else {
        this->handleMessages(messages);
      }
      continue;
    } else {
      std::string content = buf;
      long time = getTime();
      Message message = Message(user->name, content, DefaultIP, time);
      this->clientSocket->sendMessage(&message, sizeof(message));
    }

    if (flag == SOCKET_ERROR) {
      LOG_WARN("Send Server Failed: %s",buf);
      continue;
    } else {
      printf("Send to Server: %s\n", buf);
    }

    ssize_t recvLen = this->clientSocket->recvMessage(buf, 1024);
    if (recvLen == 0)
      continue;
    if (recvLen == SOCKET_ERROR) {
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

int RinaClient::handleMessages(MultiMessage *messages) {

  auto iter = messages->messages.begin();
  for (; iter != messages->messages.end(); iter ++) {
    printf("[%ld] %s(%s): %s\n", iter->getTime(), iter->getUsername(), iter->getAddress(),iter->getContent());
  }
  delete messages;
  return 0;
}

void RinaClient::createUser(std::string& name) {
  user->updateSeq = 0;
  user->name = name;
  user->sockfd = 0;
  user->state = offline;
}

//
//int RinaClient::logout() {
//  LOG_INFO("Client Stops")
//  std::string username = this->user->name;
//
//  Message message(username, LOGIN, "", );
//  this->clientSocket->sendMessage(message, strlen(message));
//  this->clientSocket->stop();
//  return 0;
//}

}