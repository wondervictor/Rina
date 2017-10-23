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




namespace Rina {


/**
 * @class SockerError
 * @brief Socket Error Info
 * */


enum SocketError{


};

//class SocketError{
//
// public:
//  SocketError() = default;
//  SocketError(int id, std::string message):id(id),message(message) {}
//  std::string& msg() const { return message; }
//
// private:
//  int id;
//  std::string message;
//
//};
//


class UserAddr {

 public:
  UserAddr(int port, sockaddr_in sockAddr, int sockfd):sockfd(sockfd),port(port),sockAddr(sockAddr){};
  int sockfd;
  int port;
  sockaddr_in sockAddr;
};


/**
 * @class Socket
 * @brief Simple Package for Socket
 * */


/**
 * @class ServerSocket
 * @brief Server Socket
 * */
class ServerSocket {

 public:
  ServerSocket()= default;
  int init(int port);
  int startServer();
  int stopServer();
  int sendMessage(int sockfd, void* buf, size_t size);
  int recvMessage(int sockfd, void* buf, size_t size);
  int acceptConn(sockaddr*clientAddr);

 private:
  int sockfd;
  int clientfds;

};


class ClientSocket {

 public:
  ClientSocket()= default;
  int init(int port);
  int conn(std::string server, int port);
  int sendMessage(void* buf, size_t size);
  int recvMessage(void* buf, size_t size);
  int stop();

 private:
  int sockfd;
  int connfd;
};

}


#endif //RINA_SOCKET_H
