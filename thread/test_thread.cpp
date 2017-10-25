/**
 * @brief Thread Test
 * @author Vic Chan
 * @date 2017-10-23
 * */

#include <string>
#include <vector>
#include "Thread.h"


struct message {
  int id;
  std::string content;
  message(int id, std::string content):id(id), content(content) {};
};

void handle(void* msg) {
  message* msg1 = (message* )msg;
  printf("id:%d msg:%s\n", msg1->id, msg1->content.c_str());
}


using namespace Rina;

int main(int argc, char* argv[]) {

  auto* manager = new ThreadManager;

  message* msg1 = new message(10, "hello");
  message* msg2 = new message(4, "world");
  message* msg3 = new message(50, "cpp");

  manager->createThread(1, handle, (void* )msg1);
  manager->createThread(2, handle, (void* )msg2);
  manager->createThread(3, handle, (void* )msg3);

  char p = getchar();

  return 0;
}


