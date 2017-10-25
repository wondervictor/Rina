/**
 * Author: Vic Chan
 * Date: 2017/10/19
 * Content: test main
 * */


#include "./Socket.h"
#include <iostream>
#include <cstring>
#include "../log/log.h"



#define CHECK(x, m, handle) if ((x) == (m)) { \
                              handle; \
                              return -1; \
                            }

using namespace Rina;

int main(int argc, char* argv[]) {

  if (argc < 2) {
    LOG_WARN("./main [type(server/client)]")
    exit(-1);
  }

  if (strcmp(argv[1], "server") == 0) {
    LOG_INFO("Run as Server")

    char buf[50];
    sockaddr_in* clientAddr;

    ServerSocket Server=ServerSocket(23333, 20);
    int sockfd = Server.init(23333);
    int start = Server.startServer();
    int accept = Server.acceptConn(clientAddr);
    long recvSize=Server.recvMessage(accept,(void*) buf,50);
    printf("%s\n",buf);
    Server.sendMessage(accept,(void*) buf,50);
    Server.stopServer();

  } else if (strcmp(argv[1], "client") == 0) {
    LOG_INFO("Run as Client")

    char buf[50];
    ClientSocket Client=ClientSocket();

    int inita = Client.init(25555);
    int con = Client.conn((const std::string&)argv[2], 23333);
    LOG_INFO("Connecting to IP: %s", argv[2])

    fgets(buf,50,stdin);
    //printf("%s\n",buf);
    int send = Client.sendMessage((void*) buf, 50);

    long recv = Client.recvMessage((void*) buf,50);
    printf("%s\n",buf);
    int st = Client.stop();

  } else {
    LOG_WARN("No run")
  }
  return 0;
}
