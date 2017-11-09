/**
 * @brief Model
 * @author Vic Chan
 * @date 2017-10-30
 * */

#include "Model.h"
#include "cJSON.h"
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

Message& Message::operator=(const Message &msg) {
  this->username = msg.username;
  this->content = msg.content;
  this->timestamp = msg.timestamp;
  this->ipAddress = msg.ipAddress;
  return *this;
}


void Message::toString(std::string &p) {

  cJSON* jsonArray = cJSON_CreateArray();
  cJSON* messageJSON = cJSON_CreateObject();
  cJSON_AddStringToObject(messageJSON, "name", this->username.c_str());
  cJSON_AddStringToObject(messageJSON, "content", this->content.c_str());
  cJSON_AddStringToObject(messageJSON, "ip", this->ipAddress.c_str());
  cJSON_AddNumberToObject(messageJSON, "timestamp", this->timestamp);
  cJSON_AddItemToArray(jsonArray, messageJSON);
  p = cJSON_Print(jsonArray);
}

Message::Message(const Message &msg) {
  this->username = msg.username;
  this->content = msg.content;
  this->timestamp = msg.timestamp;
  this->ipAddress = msg.ipAddress;
}

void MultiMessage::toString(std::string &p) {
  cJSON* jsonArray = cJSON_CreateArray();
  for (auto& msg: this->messages) {
    cJSON* msgJSON = cJSON_CreateObject();
    cJSON_AddStringToObject(msgJSON, "name", msg.getUsername().c_str());
    cJSON_AddStringToObject(msgJSON, "content", msg.getContent().c_str());
    cJSON_AddStringToObject(msgJSON, "ip", msg.getAddress().c_str());
    cJSON_AddNumberToObject(msgJSON, "timestamp", msg.getTime());
    cJSON_AddItemToArray(jsonArray, msgJSON);
  }
  p = cJSON_Print(jsonArray);
}


int parseMessage(const std::string& messageStr, std::vector<Message>& messages) {
  cJSON* root;
  root = cJSON_Parse(messageStr.c_str());
  int len = cJSON_GetArraySize(root);
  printf("Array Size: %d", len);
  cJSON* item;
  for(auto i = 0; i < len; i ++) {
    item = cJSON_GetArrayItem(root, i);
    std::string name = cJSON_GetObjectItem(item, "name")->valuestring;
    std::string content = cJSON_GetObjectItem(item, "content")->valuestring;
    std::string ip = cJSON_GetObjectItem(item, "ip")->valuestring;
    int timestamp = cJSON_GetObjectItem(item, "timestamp")->valueint;

    Message message(name, content, ip, timestamp);
    messages.push_back(message);
  }
  return (int)messages.size();
}

}