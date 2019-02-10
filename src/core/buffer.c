#include "buffer.h"

void vector_buffer_init(vector_buffer *self, int size, GLenum u) {
  glGenBuffers(1, &self->id);
  engine_gl_check();
  self->size = size;
  self->usage = u;
  self->data = (vector *)malloc(sizeof(vector) * size);
}

void vector_buffer_destroy(vector_buffer *self) {
  glDeleteBuffers(1, &self->id);
  free(self->data);
  engine_gl_check();
}

void vector_buffer_bind(vector_buffer *self, int index) {

  glBindBuffer(GL_ARRAY_BUFFER, self->id);
  engine_gl_check();
  glEnableVertexAttribArray(index);
  engine_gl_check();
  glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, 0);
  engine_gl_check();

  // glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
  // glEnableClientState(GL_vector_buffer);
  // glVertexPointer(3, GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(0));    // The
  // starting point of the VBO, for the vertices
}

void vector_buffer_update(vector_buffer *self, int size) {
  glBindBuffer(GL_ARRAY_BUFFER, self->id);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(vector) * (size == -1 ? self->size : size),
               &self->data[0], self->usage);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  engine_gl_check();
}

void point_buffer_init(point_buffer *self, int size, GLenum u) {
  glGenBuffers(1, &self->id);
  engine_gl_check();
  self->size = size;
  self->usage = u;
  self->data = (point *)malloc(sizeof(vector) * size);
}

void point_buffer_destroy(point_buffer *self) {
  glDeleteBuffers(1, &self->id);
  free(self->data);
  engine_gl_check();
}

void point_buffer_bind(point_buffer *self, int index) {

  glBindBuffer(GL_ARRAY_BUFFER, self->id);
  engine_gl_check();
  glEnableVertexAttribArray(index);
  engine_gl_check();
  glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, 0, 0);
  engine_gl_check();

  // glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
  // glEnableClientState(GL_point_buffer);
  // glVertexPointer(3, GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(0));    // The
  // starting point of the VBO, for the vertices
}

void point_buffer_update(point_buffer *self, int size) {
  glBindBuffer(GL_ARRAY_BUFFER, self->id);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(point) * (size == -1 ? self->size : size), &self->data[0],
               self->usage);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  engine_gl_check();
}
