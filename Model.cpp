/**
 * @brief Model
 * @author Vic Chan
 * @date 2017-10-30
 * */

#include "Model.h"
#include "cJSON.h"
#include <ctime>
#include <string>
#include <cstdlib>

namespace Rina {

MessageType Message::getType() {
  std::string content = this->content;
  if (content == LOGIN) {
    return login;
  } else if (content == LOGOUT) {
    return logout;
  } else if (content == GET_ALL) {
    return getall;
  } else if (content == LOGIN_SUCCESS){
    return success;
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


std::string Message::toString() {

  cJSON* jsonArray = cJSON_CreateArray();
  cJSON* messageJSON = cJSON_CreateObject();
  cJSON_AddStringToObject(messageJSON, "name", this->username.c_str());
  cJSON_AddStringToObject(messageJSON, "content", this->content.c_str());
  cJSON_AddStringToObject(messageJSON, "ip", this->ipAddress.c_str());
  cJSON_AddNumberToObject(messageJSON, "timestamp", this->timestamp);
  cJSON_AddItemToArray(jsonArray, messageJSON);
  std::string p = cJSON_Print(jsonArray);
  cJSON_Delete(jsonArray);
  return p;
}

Message::Message(const Message &msg) {
  this->username = msg.username;
  this->content = msg.content;
  this->timestamp = msg.timestamp;
  this->ipAddress = msg.ipAddress;
}

std::string MultiMessage::toString() {
  cJSON* jsonArray = cJSON_CreateArray();
  printf("MSGS: %d\n", messages.size());
  for (auto& msg: this->messages) {
    cJSON* msgJSON = cJSON_CreateObject();
    cJSON_AddItemToArray(jsonArray, msgJSON);
    cJSON_AddStringToObject(msgJSON, "name", msg.getUsername().c_str());
    cJSON_AddStringToObject(msgJSON, "content", msg.getContent().c_str());
    cJSON_AddStringToObject(msgJSON, "ip", msg.getAddress().c_str());
    cJSON_AddNumberToObject(msgJSON, "timestamp", msg.getTime());
  }
  std::string p = cJSON_Print(jsonArray);
  cJSON_Delete(jsonArray);
  return p;
}


int parseMessage(char* str, std::vector<Message>& messages) {
  printf("Message: %s\n", str);
  cJSON* root;
  root = cJSON_Parse(str);
  int len = cJSON_GetArraySize(root);
  printf("Array Size: %d\n", len);
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
  cJSON_Delete(root);
  return (int)messages.size();
}

}