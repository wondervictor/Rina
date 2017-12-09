/**
 * @brief Log
 * @author Vic Chan
 * @date 2017-10-23
 * */



#ifndef RINA_LOG_H
#define RINA_LOG_H
#include <stdio.h>
#include <errno.h>

#define NONE      "\e[0m"
#define BLACK     "\e[0;30m"
#define RED       "\e[0;31m"
#define GREEN     "\e[0;32m"
#define BROWN     "\e[0;33m"
#define BLUE      "\e[0;34m"
#define PURPLE    "\e[0;35m"
#define CYAN      "\e[0;36m"
#define GRAY      "\e[0;37m"

#define WARN        "[WARNING]"
#define INFO        "[INFO]"
#define ERROR       "[ERROR]"
#define NEW_LINE    "\n"

#define LOG_WARN(...)   { printf(PURPLE); \
                          printf(__TIME__); \
                          printf(WARN);\
                          printf(NONE); \
                          printf(__VA_ARGS__);   \
                          printf(NEW_LINE); \
                        }

#define LOG_ERROR(...)  { printf(RED); \
                          printf(__TIME__); \
                          printf(ERROR);\
                          printf(NONE); \
                          printf(__VA_ARGS__);   \
                          printf(NEW_LINE); \
                        }


#define LOG_INFO(...)   { printf(GREEN); \
                          printf(__TIME__); \
                          printf(INFO); \
                          printf(NONE); \
                          printf(__VA_ARGS__);   \
                          printf(NEW_LINE); \
                        }

#define DEBUG(x)        { if (errno > 0) printf("%s[ERROR]%s %s error: %s (errno: %d)\n", RED, NONE, x, strerror(errno), errno);}

#endif //RINA_LOG_H
