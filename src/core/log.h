#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#define app_log(...)                                                               \
  {                                                                            \
    printf(__VA_ARGS__);                                                       \
    printf("\n");                                                              \
  }

#define app_warning(...)                                                           \
  {                                                                            \
    printf("WARNING: ");                                                       \
    printf(__VA_ARGS__);                                                       \
    printf("\n");                                                              \
  }


#ifdef DEBUG
#define app_debug(...)                                                      \
  {                                                                            \
    printf("DEBUG: ");                                                         \
    printf(__VA_ARGS__);                                                  \
    printf("\n");                                                              \
  }
#else
#define app_debug(s, ...)
#endif

#endif