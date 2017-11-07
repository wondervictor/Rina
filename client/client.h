/**
 * @brief Client Application
 * @author Vic Chan
 * @date 2017-10-23
 * */



#ifndef RINA_CLIENT_H
#define RINA_CLIENT_H
#include <string>
#include "Socket.h"
#include "Thread.h"
#include "Model.h"


namespace Rina {

class RinaClient {

 public:
  RinaClient() = default;
  int init(int port);
  void createUser(std::string name, )
  int login(std::string&, int);
  int logout();

 private:
  User* user;
  // 连接 Center 服务器
  ClientSocket* clientSocket;
  // Peer to Peer 连接
  ServerSocket* peerSocket;
  ThreadManager* manager;

};

}

#endif //RINA_CLIENT_H
