#include "buffer.h"

void vector_buffer_init(TW_VectorBuffer *self, int size, GLenum u) {
  glGenBuffers(1, &self->id);
  engine_gl_check();
  self->size = size;
  self->usage = u;
  self->data = (TW_Vector *)malloc(sizeof(TW_Vector) * size);
}

TW_VectorBuffer *vector_buffer_create(int size, GLenum u) {
  TW_VectorBuffer *v = (TW_VectorBuffer *)malloc(sizeof(TW_VectorBuffer));
  vector_buffer_init(v, size, u);
  return v;
}

void vector_buffer_destroy(TW_VectorBuffer *self) {
  glDeleteBuffers(1, &self->id);
  free(self->data);
  engine_gl_check();
}

void vector_buffer_bind(TW_VectorBuffer *self, int index) {

  glBindBuffer(GL_ARRAY_BUFFER, self->id);
  engine_gl_check();
  glEnableVertexAttribArray(index);
  engine_gl_check();
  glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, 0);
  engine_gl_check();

  // glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
  // glEnableClientState(GL_vector_buffer);
  // glVertexPointer(3, GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(0));    // The
  // starting TW_Point of the VBO, for the vertices
}

void vector_buffer_update(TW_VectorBuffer *self, int size) {
  glBindBuffer(GL_ARRAY_BUFFER, self->id);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(TW_Vector) * (size == -1 ? self->size : size),
               &self->data[0], self->usage);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  engine_gl_check();
}

void point_buffer_init(TW_PointBuffer *self, int size, GLenum u) {
  glGenBuffers(1, &self->id);
  engine_gl_check();
  self->size = size;
  self->usage = u;
  self->data = (TW_Point *)malloc(sizeof(TW_Point) * size);
}

TW_PointBuffer *point_buffer_create(int size, GLenum u) {
  TW_PointBuffer *v = (TW_PointBuffer *)malloc(sizeof(TW_PointBuffer));
  point_buffer_init(v, size, u);
  return v;
}

void point_buffer_destroy(TW_PointBuffer *self) {
  glDeleteBuffers(1, &self->id);
  free(self->data);
  engine_gl_check();
}

void point_buffer_bind(TW_PointBuffer *self, int index) {

  glBindBuffer(GL_ARRAY_BUFFER, self->id);
  engine_gl_check();
  glEnableVertexAttribArray(index);
  engine_gl_check();
  glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, 0, 0);
  engine_gl_check();

  // glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
  // glEnableClientState(GL_point_buffer);
  // glVertexPointer(3, GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(0));    // The
  // starting TW_Point of the VBO, for the vertices
}

void point_buffer_update(TW_PointBuffer *self, int size) {
  glBindBuffer(GL_ARRAY_BUFFER, self->id);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(TW_Point) * (size == -1 ? self->size : size),
               &self->data[0], self->usage);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  engine_gl_check();
}
