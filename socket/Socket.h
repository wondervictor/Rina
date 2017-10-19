/**
 * Author: Vic Chan
 * Date: 2017/10/13
 *
 **/

#ifndef RINA_SOCKET_H
#define RINA_SOCKET_H
#include <string>
#include <netdb.h>
#include <cstdio>
#include <sys/socket.h>



namespace Rina {


/**
 * @class SockerError
 * @brief Socket Error Info
 * */
class SocketError{

 public:
  SocketError() = default;
  SocketError(int id, std::string message):id(id),message(message) {}
  std::string& msg() const { return message; }

 private:
  int id;
  std::string message;

};



class UserAddr {

};

/**
 * @class Message
 * @brief 定义发送消息
 * */
class Message {

 public:
  size_t size() const { return msgSize; }
  Message(void* buf, size_t bufSize):msgSize(bufSize),buf(buf) {};

 private:
  size_t msgSize;
  void* buf;

};

/**
 * @class Socket
 * @brief Simple Package for Socket
 * */
class Socket {
 public:

  Socket();
  virtual void init(SocketError* error);
  virtual void send(const Message& buf, const UserAddr& user, SocketError* error);
  virtual void recv(Message& buf, const UserAddr& user, SocketError* error);
  virtual ~Socket();

};

class ServerSocket: public Socket {

 public:
  ServerSocket()= default;
  void init(SocketError* error);
  void startServer(SocketError* error);
  void stopServer(SocketError* error);
  void send(const Message& buf, const UserAddr& user, SocketError* error);
  void recv(Message& buf, const UserAddr& user, SocketError* error);

 private:
  sockaddr_in serverAddr;

};


class ClientSocket: public Socket {

 public:
  ClientSocket()= default;
  void init(SocketError* error);
  void connect(const UserAddr& serverAddr, SocketError* error)
  void send(const Message& buf, const UserAddr& user, SocketError* error);
  void recv(Message& buf, const UserAddr& user, SocketError* error);

  void stop(SocketError* error);

 private:
  sockaddr_in serverAddr;


};

}


#endif //RINA_SOCKET_H
