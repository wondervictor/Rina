/**
 * @brief Client Application
 * @author Vic Chan
 * @date 2017-10-23
 * */



#ifndef RINA_CLIENT_H
#define RINA_CLIENT_H
#include <string>
#include "../socket/Socket.h"
#include "../thread/Thread.h"


namespace Rina {

class RinaClient {

 public:
  RinaClient() = default;
  int init(int port);
  int login(std::string&, int);
  int logout();

 private:
  ClientSocket* clientSocket;
  ThreadManager* manager;

  void handleRecvMesage(void* );
  void handleSendMessage(void* );

};

}

#endif //RINA_CLIENT_H
