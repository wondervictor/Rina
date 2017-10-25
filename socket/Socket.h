/**
 * @brief Socket
 * @author Vic Chan
 * @date 2017-10-23
 * */


#ifndef RINA_SOCKET_H
#define RINA_SOCKET_H
#include <string>
#include <netdb.h>
#include <sys/socket.h>
#include <vector>

#define CHECK(x, m, handle) if ((x) == (m)) { \
                              handle; \
                              return -1; \
                            }

#define SOCKET_ERROR  (-1)


namespace Rina {


/**
 * @class SockerError
 * @brief Socket Error Info
 * */

enum SocketError{


};


enum SocketState {
  Uninitialized=0,
  Running=1,
  Stopped=2,
  Error=3
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
  std::vector<int> getClients();
  SocketState getState() const { return state; }


 private:
  SocketState state;
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
  SocketState getState() const { return state; }

 private:
  SocketState state;
  sockaddr_in clientAddr;
  sockaddr_in connectAddr;
  int sockfd;
  int port;
};

}


#endif //RINA_SOCKET_H
