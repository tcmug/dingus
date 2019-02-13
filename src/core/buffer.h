#ifndef POINT_BUFFER_H
#define POINT_BUFFER_H

#include "../core/engine.h"
#include "../math/math.h"

typedef struct vector_buffer_t {

  GLuint id;
  GLuint vaid;
  GLenum usage;

  int size;
  TW_Vector *data;

} TW_VectorBuffer;

typedef struct point_buffer_t {

  GLuint id;
  GLenum usage;

  int size;
  TW_Point *data;
} TW_PointBuffer;

void TW_PointBufferInit(TW_PointBuffer *self, int size, GLenum u);
TW_PointBuffer *TW_PointBufferCreate(int size, GLenum u);
void TW_PointBufferDestroy(TW_PointBuffer *self);
void TW_PointBufferBind(TW_PointBuffer *self, int index);
void TW_PointBufferUpdate(TW_PointBuffer *self, int size);

void TW_VectorBufferInit(TW_VectorBuffer *self, int size, GLenum u);
TW_VectorBuffer *TW_VectorBufferCreate(int size, GLenum u);
void TW_VectorBufferDestroy(TW_VectorBuffer *self);
void TW_VectorBufferBind(TW_VectorBuffer *self, int index);
void TW_VectorBufferUpdate(TW_VectorBuffer *self, int size);

#endif