/**
 * Author: Vic Chan
 * Date: 2017/10/13
 *
 **/

#ifndef RINA_SOCKET_H
#define RINA_SOCKET_H
#include <string>
#include <netdb.h>
#include <sys/socket.h>
#include <vector>

namespace Rina {


/**
 * @class SockerError
 * @brief Socket Error Info
 * */

enum SocketError{


};


/**
 * @class ServerSocket
 * @brief Server Socket Package
 * */
class ServerSocket {

 public:
  ServerSocket()= default;
  ServerSocket(int port, int maxConn):port(port), maxConn(maxConn) {};
  int init(int port);
  int startServer();
  int stopServer();
  int sendMessage(int sockfd, void* buf, size_t size);
  long recvMessage(int sockfd, void* buf, size_t size);
  int acceptConn(sockaddr_in* clientAddr);
  int broadcast(void* buf, size_t size);

 private:
  sockaddr_in serverAddr;
  int sockfd;
  int port;
  std::vector<int> clients;
  int maxConn;

};


/**
 * @class ClientSocket
 * @brief Package for client socket
 * */
class ClientSocket {

 public:
  ClientSocket()= default;
  int init(int port);
  int conn(const std::string& server, int port);
  int sendMessage(void* buf, size_t size);
  long recvMessage(void* buf, size_t size);
  int stop();

 private:
  sockaddr_in clientAddr;
  sockaddr_in connectAddr;
  int sockfd;
  int port;
};

}


#endif //RINA_SOCKET_H
