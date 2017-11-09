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

std::string ServerName = "SERVER";
std::string DefaultIP = "0.0.0.0";

struct __handle {
  int fd;
  ServerSocket* serverSocket;
  RinaServer* server;
};

static void handle(void* handle) {

  auto* clientHandle = (__handle* )handle;
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
      long timestamp = getTime();
      std::string responseMsg = LOGIN_SUCCESS;
      Message response(ServerName, responseMsg, DefaultIP, timestamp);
      clientHandle->serverSocket->sendMessage(sockfd, &response, sizeof(response));
    } else if (msgBuf.getType() == logout) {
      std::string username = msgBuf.getUsername();
      User tmpUser;
      tmpUser.sockfd = sockfd;
      tmpUser.name = username;
      clientHandle->server->logout(tmpUser);

      //long timestamp = getTime();
      //std::string responseMsg = LOGOUT;
      //Message response(ServerName, responseMsg, DefaultIP, timestamp);
      //clientHandle->serverSocket->sendMessage(sockfd, &response, sizeof(response));

    } else if (msgBuf.getType() == getall) {

      User user = clientHandle->server->getUser(sockfd);
      long updateSeq = user.updateSeq;
      std::vector<Message> messages;
      clientHandle->server->getMessages(updateSeq, messages);
      long messagesCount = messages.size();
      updateSeq += messagesCount;
      user.updateSeq = updateSeq;
      clientHandle->server->updateUser(sockfd, user);
      MultiMessage message(messages);
      clientHandle->serverSocket->sendMessage(sockfd, &message, sizeof(message));
    } else {
      clientHandle->server->addMessage(msgBuf);
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

int RinaServer::logout(User& user) {
  LOG_INFO("User: %s Log Out", user.name.c_str())
  this->serverSocket->closeConn(user.sockfd);
  this->userAddrs.erase(user.sockfd);
  this->users.erase(user.sockfd);
  return 0;
}

int RinaServer::getMessages(int seq, std::vector<Message>& msgs) {
  auto msgIter = this->messages.begin();
  msgIter += seq;
  for(; msgIter != this->messages.end(); msgIter ++) {
    msgs.push_back(**msgIter);
  }
  return 0;
}

User RinaServer::getUser(int sockfd) {
  return this->users[sockfd];
}

std::map<int, User> RinaServer::getUserMaps() {
  return users;
}

int RinaServer::updateUser(int sockfd, User& user) {
  this->users[sockfd] = user;
  return 0;
}

int RinaServer::login(User& user, std::string& userIP) {
  this->users[user.sockfd] = user;
  LOG_INFO("User: %s from IP: %s joins the room now", user.name.c_str(), userIP.c_str());
  return 0;
}

int RinaServer::addMessage(Message &msg) {
#warning add thread lock here
  auto* newMsg = new Message(msg);
  this->messages.push_back(newMsg);
  return 0;
}

RinaServer::~RinaServer() {
  delete serverSocket;
  for(Message* p: this->messages) {
    delete p;
  }
}

}