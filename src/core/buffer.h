#ifndef POINT_BUFFER_H
#define POINT_BUFFER_H

#include "../core/engine.h"
#include "../math/math.h"

typedef struct s_vector_buffer {

  GLuint id;
  GLenum usage;

  int size;
  vector *data;
} vector_buffer;

typedef struct s_point_buffer {

  GLuint id;
  GLenum usage;

  int size;
  point *data;
} point_buffer;

void point_buffer_init(point_buffer *self, int size, GLenum u);
void point_buffer_destroy(point_buffer *self);
void point_buffer_bind(point_buffer *self, int index);
void point_buffer_update(point_buffer *self, int size);

void vector_buffer_init(vector_buffer *self, int size, GLenum u);
void vector_buffer_destroy(vector_buffer *self);
void vector_buffer_bind(vector_buffer *self, int index);
void vector_buffer_update(vector_buffer *self, int size);

#endif