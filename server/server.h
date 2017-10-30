/**
 * @brief Server Application
 * @author Vic Chan
 * @date 2017-10-23
 * */



#ifndef RINA_SERVER_H
#define RINA_SERVER_H

#include <map>
#include <vector>

#include "../socket/Socket.h"
#include "../thread/Thread.h"


namespace Rina {

class message {
  std::string user;
  std::string content;
  long timestamp;
};


class ClientUser {
  int sockfd;
  std::string name;
  int state = 0;

};

/**
 * @brief Rina Server
 * */
class RinaServer {

 public:
  RinaServer()= default;


  int init(int port);
  int start();
  int stop();
  int login();
  int logout();


 private:
  ThreadManager threadManager;
  std::map<int, sockaddr_in> users;
  ServerSocket* serverSocket;
  std::vector<message *> messages;


};

}


#endif //RINA_SERVER_H
