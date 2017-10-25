/**
 * @brief Server Application
 * @author Vic Chan
 * @date 2017-10-23
 * */

#include "server.h"
#include "../log/log.h"


namespace Rina {

int RinaServer::init(int port) {
  LOG_INFO("Init Server")
  this->serverSocket = new ServerSocket();
  int flag = this->serverSocket->init(port);
  CHECK(flag, SOCKET_ERROR, LOG_WARN("Fail Init Server"))
  LOG_INFO("Finish Init Server")
  return 0;
}

int RinaServer::start() {



}



}