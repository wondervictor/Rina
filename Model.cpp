/**
 * @brief Model
 * @author Vic Chan
 * @date 2017-10-30
 * */

#include "Model.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/writer.h"
#include <ctime>
#include <string>
#include <cstdlib>

namespace Rina {

using rapidjson::Document;
using rapidjson::Value;
using rapidjson::Writer;
using rapidjson::StringBuffer;

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

  Document jsonDoc;
  Document::AllocatorType &allocator = jsonDoc.GetAllocator();
  jsonDoc.SetArray();
  Value contentObj(rapidjson::kObjectType);
  contentObj.AddMember("name", this->username, jsonDoc.GetAllocator());
  contentObj.AddMember("content", this->content, allocator);
  contentObj.AddMember("ip", this->ipAddress, allocator);
  contentObj.AddMember("timestamp", this->timestamp, allocator);
  jsonDoc.PushBack(contentObj, allocator);
  StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);
  jsonDoc.Accept(writer);
  std::string p= buffer.GetString();

  return p;
}

Message::Message(const Message &msg) {
  this->username = msg.username;
  this->content = msg.content;
  this->timestamp = msg.timestamp;
  this->ipAddress = msg.ipAddress;
}

std::string MultiMessage::toString() {

  Document jsonDoc;
  Document::AllocatorType &allocator = jsonDoc.GetAllocator();
  jsonDoc.SetArray();
  printf("MSGS: %ld\n", messages.size());
  for (auto& msg: this->messages) {
    Value contentObj(rapidjson::kObjectType);
    contentObj.AddMember("name", msg.getUsername(), jsonDoc.GetAllocator());
    contentObj.AddMember("content", msg.getContent(), allocator);
    contentObj.AddMember("ip", msg.getAddress(), allocator);
    contentObj.AddMember("timestamp", msg.getTime(), allocator);
    jsonDoc.PushBack(contentObj, allocator);

  }
  StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);
  jsonDoc.Accept(writer);
  std::string p= buffer.GetString();

  return p;
}


int parseMessage(char* str, std::vector<Message>& messages) {
  printf("Message: %s\n", str);

  Document jsonDoc;
  //FileReadStream inputBuffer(inputStr);
  jsonDoc.Parse(str);
  if (jsonDoc.HasParseError()) {
    printf("Parse Error");
    return -1;
  }

  if (jsonDoc.IsArray() && !jsonDoc.Empty()) {
    jsonDoc.SetArray();
    for (rapidjson::SizeType i = 0; i < jsonDoc.Size(); i ++) {
      const Value& object = jsonDoc[i];
      std::string name = object["name"].GetString();
      std::string content = object["content"].GetString();
      std::string ip = object["ip"].GetString();
      int timestamp = object["timestamp"].GetInt();
      Message message(name, content, ip, timestamp);
      messages.push_back(message);
    }
  }

  return (int)messages.size();
}

}