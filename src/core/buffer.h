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

void point_buffer_init(TW_PointBuffer *self, int size, GLenum u);
TW_PointBuffer *point_buffer_create(int size, GLenum u);
void point_buffer_destroy(TW_PointBuffer *self);
void point_buffer_bind(TW_PointBuffer *self, int index);
void point_buffer_update(TW_PointBuffer *self, int size);

void vector_buffer_init(TW_VectorBuffer *self, int size, GLenum u);
TW_VectorBuffer *vector_buffer_create(int size, GLenum u);
void vector_buffer_destroy(TW_VectorBuffer *self);
void vector_buffer_bind(TW_VectorBuffer *self, int index);
void vector_buffer_update(TW_VectorBuffer *self, int size);

#endif