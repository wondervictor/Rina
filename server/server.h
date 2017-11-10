/**
 * @brief Server Application
 * @author Vic Chan
 * @date 2017-10-23
 * */



#ifndef RINA_SERVER_H
#define RINA_SERVER_H

#include <map>
#include <vector>
#include "Socket.h"
#include "Thread.h"
#include "Model.h"


namespace Rina {

/**
 * @brief Rina Server
 * */
class RinaServer {

 public:
  RinaServer()= default;

  int init(int port);
  int start();
  int stop();
  int login(User& user, std::string& userIP);
  int logout(User& user);
  User getUser(int sockfd);
  std::map<int, User> getUserMaps();
  int updateUser(int sockfd, User& user);
  int getMessages(int seq, std::vector<Message>&);
  int addMessage(Message&);
  ~RinaServer();

 private:
  ThreadManager threadManager;
  std::map<int, User> users;
  std::map<int, sockaddr_in> userAddrs;
  ServerSocket* serverSocket;
  std::vector<Message *> messages;

};

}


#endif //RINA_SERVER_H
