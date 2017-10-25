/**
 * @brief Server Application
 * @author Vic Chan
 * @date 2017-10-23
 * */



#ifndef RINA_SERVER_H
#define RINA_SERVER_H

#include "../socket/Socket.h"
#include "../thread/Thread.h"

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
  int login();
  int logout();


 private:
  ThreadManager threadManager;
  void handle();
  ServerSocket* serverSocket;

};

}


#endif //RINA_SERVER_H
