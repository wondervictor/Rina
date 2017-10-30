/**
 * @brief Model
 * @author Vic Chan
 * @date 2017-10-30
 * */


#ifndef RINA_MODEL_H
#define RINA_MODEL_H
#include <string>
#include <vector>

// 登录
#define LOGIN         "*2c411521vn148-e=1r"
// 登出
#define LOGOUT        "e21874vnwv1o2870810"
// 请求聊天记录
#define GET_ALL       "1321c32-23@(*^&*#Bv"

namespace Rina {


enum UserState {
  online = 0,
  offline = 1
};

enum MessageType {
  normal = 0,
  login,
  logout,
  getall
};

class Message {
 public:
  Message(std::string& _name, std::string& _content, std::string& ip, long time):
      username(_name),
      content(_content),
      ipAddress(ip),
      timestamp(time){};
  std::string getUsername() const { return username; }
  std::string getContent() const { return content; }
  std::string getAddress() const { return ipAddress; }
  inline MessageType getType();
  long getTime() const { return timestamp; }

 private:
  std::string username;
  std::string content;
  std::string ipAddress;
  long timestamp;
};

class MultiMessage {
 public:
  std::vector<Message> messages;

};



class ClientUser {
  int sockfd;
  std::string name;
  UserState state = offline;
  long updateSeq = 0;
};


}

#endif //RINA_MODEL_H
