/**
 * @brief Thread Package
 * @author Vic Chan
 * @date 2017-10-23
 * */


#include "./thread.h"
#include "log.h"

namespace Rina {

ThreadManager::ThreadManager() {

}

void ThreadManager::createThread(int key, void (*handle)(void *), void *data) {
  threadNum ++;
  if (threads[key] != nullptr) {
    LOG_WARN("New Thread will Over the Existing Thread");
  }
  LOG_INFO("Thread")
  auto* newThread = new std::thread(handle, data);
  threads[key] = newThread;
}




}