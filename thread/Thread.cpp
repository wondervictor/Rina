/**
 * @brief Thread Package
 * @author Vic Chan
 * @date 2017-10-23
 * */


#include "Thread.h"
#include "../log/log.h"

namespace Rina {

ThreadManager::ThreadManager() {

}

void ThreadManager::createThread(int key, void (*handle)(void *), void *data) {
  std::thread* newThread = new std::thread(handle, data);
  threadNum ++;
  if (threads[key] != nullptr) {
    LOG_WARN("New Thread will Over the Existing Thread");
  }
  threads[key] = newThread;
}




}