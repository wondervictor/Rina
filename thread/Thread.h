/**
 * @brief Thread Package
 * @author Vic Chan
 * @date 2017-10-23
 * */


#ifndef RINA_THREAD_H
#define RINA_THREAD_H

#include <thread>
#include <map>

namespace Rina {

class ThreadManager {

 public:
  ThreadManager();

  void createThread(int key, void( *handle )( void* ), void* data);
  std::thread* getThread(int key) { return threads[key]; }


 private:

  std::map<int, std::thread* > threads;
  int threadNum;





};

}

#endif //RINA_THREAD_H
