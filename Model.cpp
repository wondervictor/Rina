/**
 * @brief Model
 * @author Vic Chan
 * @date 2017-10-30
 * */

#include "Model.h"
#include <time.h>
#include <stdlib.h>

namespace Rina {

MessageType Message::getType() {
  std::string content = this->content;
  if (content == LOGIN) {
    return login;
  } else if (content == LOGOUT) {
    return logout;
  } else if (content == GET_ALL) {
    return getall;
  } else {
    return normal;
  }
}

Message& Message::operator=(Message &msg) {
  this->username = msg.username;
  this->content = msg.content;
  this->timestamp = msg.timestamp;
  this->ipAddress = msg.ipAddress;
  return *this;
}

Message::Message(Message &msg) {
  this->username = msg.username;
  this->content = msg.content;
  this->timestamp = msg.timestamp;
  this->ipAddress = msg.ipAddress;
}

inline long getTime() {
  long timestamp = time(nullptr);
  return timestamp;
}

}