//
// Created by VicChan on 2017/10/19.
//

#ifndef RINA_LOG_H
#define RINA_LOG_H
#include <stdio.h>

#define NONE      "\e[0m"
#define BLACK     "\e[0;30m"
#define RED       "\e[0;31m"
#define GREEN     "\e[0;32m"
#define BROWN     "\e[0;33m"
#define BLUE      "\e[0;34m"
#define PURPLE    "\e[0;35m"
#define CYAN      "\e[0;36m"
#define GRAY      "\e[0;37m"

#define WARN      "[WARNING]"
#define INFO      "[WARNING]"
#define NEW_LINE  "\n"

#define LOG_WARN(format, ...)   ({  printf(RED); \
                                    printf(__TIME__); \
                                    printf(WARN);\
                                    printf(NONE); \
                                    printf(format,__VA_ARGS__);   \
                                    printf(NEW_LINE); \
                                })

#define LOG_INFO(format, ...)   ({  printf(GREEN); \
                                    printf(__TIME__); \
                                    printf(INFO); \
                                    printf(NONE); \
                                    printf(format, __VA_ARGS__);   \
                                    printf(NEW_LINE); \
                                })

#endif //RINA_LOG_H
