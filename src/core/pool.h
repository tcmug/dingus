#ifndef TW_POOL_H
#define TW_POOL_H

typedef struct TW_Pool_t {
  unsigned char *chunk;
  unsigned char **free;
  int obj_size;
  int size;
  int free_objects;
} TW_Pool;

void TW_Pool_Init(TW_Pool *pool, int size, int obj_size);
void *TW_Pool_Alloc(TW_Pool *pool);
void TW_Pool_Free(TW_Pool *pool, void *ptr);

#endif