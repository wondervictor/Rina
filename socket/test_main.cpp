/**
 * Author: Vic Chan
 * Date: 2017/10/19
 * Content: test main
 * */


//#include <Socket.h>
#include <iostream>
#include <cstring>
#include "../log/log.h"

#define CHECK(x, m, handle) if ((x) == (m)) { \
                              handle; \
                              return -1; \
                            }


int main(int argc, char* argv[]) {

  if (argc < 2) {
    LOG_WARN("./main [type(server/client)]")
    exit(-1);
  }

  if (strcmp(argv[1], "server") == 0) {
    LOG_INFO("Run as Server")




  } else if (strcmp(argv[1], "client") == 0) {
    LOG_INFO("Run as Client")
  } else {
    LOG_WARN("No run")
  }
  return 0;
}