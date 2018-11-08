#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#define log(...)                                                               \
  {                                                                            \
    printf(__VA_ARGS__);                                                       \
    printf("\n");                                                              \
  }

#ifdef DEBUG
#define log_debug(s, ...)                                                      \
  {                                                                            \
    printf("DEBUG: ");                                                         \
    printf((s), __VA_ARGS__);                                                  \
    printf("\n");                                                              \
  }
#else
#define log_debug(s, ...)
#endif

#endif