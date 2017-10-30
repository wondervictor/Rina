/**
 * @brief Model
 * @author Vic Chan
 * @date 2017-10-30
 * */


#ifndef RINA_MODEL_H
#define RINA_MODEL_H
#include <string>


namespace Rina {


enum UserState {
  online = 0,
  offline = 1
};

class message {
 public:
  message(std::string& _name, std::string& _content, std::string& ip, long time):
      username(_name),
      content(_content),
      ipAddress(ip),
      timestamp(time){};
  std::string getUsername() const { return username; }
  std::string getContent() const { return content; }
  std::string getAddress() const { return ipAddress; }
  long getTime() const { return timestamp; }

 private:
  std::string username;
  std::string content;
  std::string ipAddress;
  long timestamp;
};


class ClientUser {
  int sockfd;
  std::string name;
  UserState state = offline;
  long updateSeq = 0;
};


}

#endif //RINA_MODEL_H
