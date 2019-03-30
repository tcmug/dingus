
#include "pool.h"

#include <assert.h>
#include <stdlib.h>

void TW_Pool_Init(TW_Pool *pool, int size, int obj_size) {
  pool->chunk = malloc(obj_size * size);
  pool->free = malloc(sizeof(unsigned char *) * size);
  pool->free_objects = size;
  pool->size = size;
  for (int i = 0; i < size; i++) {
    pool->free[size - 1 - i] = pool->chunk + (i * obj_size);
  }
}

void *TW_Pool_Alloc(TW_Pool *pool) {
  if (pool->free_objects == 0)
    return 0;
  return pool->free[--pool->free_objects];
}

void TW_Pool_Free(TW_Pool *pool, void *ptr) {
  assert(pool->size >= pool->free_objects);
  pool->free[pool->free_objects++] = ptr;
  printf("feeing %p\n", ptr);
}

// typedef struct TW_InList {
//   void *start;
//   void *end;
//   int size, top;
// } TW_InList;

// #define _TW_DATA(system, type_t, index) &((type_t *)(system).start)[(index)]

// #define TW_INLIST_ALLOC(system, type_t)                                        \
//   ({                                                                           \
//     if ((system).top == (system).size) {                                       \
//       (system).size += INLIST_ALLOC_GRANULARITY;                               \
//       (system).start =                                                         \
//           realloc((system).start, (system).size * sizeof(type_t));             \
//     }                                                                          \
//     type_t *c = _TW_DATA(system, type_t, system.top++);                        \
//     (system).end = c;                                                          \
//     c;                                                                         \
//   })

// #define TW_INLIST_COMPACT(system, type_t, freeable)                            \
//   ({                                                                           \
//     type_t *item = (system).start;                                             \
//     while (item != (system).end) {                                             \
//       if (freeable) {                                                          \
//         memmove(item, item + 1, (char *)system.end - (char *)item);            \
//         (system).end--;                                                        \
//         (system).top--;                                                        \
//       } else                                                                   \
//         item++;                                                                \
//     }                                                                          \
//   })

// #define TW_INLIST_WALK(system, type_t, callback)                               \
//   ({                                                                           \
//     for (type_t *t = system.start; t != system.end; t++) {                     \
//       callback(t);                                                             \
//     }                                                                          \
//   })

// #define TW_INLIST_BINARY_SEARCH_BY_FIELD(system, type_t, field, field_value)   \
//   ({                                                                           \
//     type_t *l = (type_t *)(system).start;                                      \
//     type_t *r = (type_t *)(system).end - 1;                                    \
//     type_t *match = 0;                                                         \
//     while (l <= r) {                                                           \
//       type_t *m = l + ((r - l) / 2);                                           \
//       if (m->field < field_value) {                                            \
//         l = m + 1;                                                             \
//       } else if (m->field > field_value) {                                     \
//         r = m - 1;                                                             \
//       } else {                                                                 \
//         match = m;                                                             \
//         break;                                                                 \
//       }                                                                        \
//     }                                                                          \
//     match;                                                                     \
//   })
