#include "buffer.h"

void TW_Vector3BufferInit(TW_Vector3Buffer *self, int size, GLenum u) {
  glGenBuffers(1, &self->id);
  engine_gl_check();
  self->size = size;
  self->usage = u;
  self->data = (TW_Vector3 *)malloc(sizeof(TW_Vector3) * size);
}

TW_Vector3Buffer *TW_Vector3BufferCreate(int size, GLenum u) {
  TW_Vector3Buffer *v = (TW_Vector3Buffer *)malloc(sizeof(TW_Vector3Buffer));
  TW_Vector3BufferInit(v, size, u);
  return v;
}

void TW_Vector3BufferDestroy(TW_Vector3Buffer *self) {
  glDeleteBuffers(1, &self->id);
  free(self->data);
  engine_gl_check();
}

void TW_Vector3BufferBind(TW_Vector3Buffer *self, int index) {

  glBindBuffer(GL_ARRAY_BUFFER, self->id);
  engine_gl_check();
  glEnableVertexAttribArray(index);
  engine_gl_check();
  glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, 0);
  engine_gl_check();

  // glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
  // glEnableClientState(GL_vector_buffer);
  // glVertexPointer(3, GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(0));    // The
  // starting TW_Vector2 of the VBO, for the vertices
}

void TW_Vector3BufferUpdate(TW_Vector3Buffer *self, int size) {
  glBindBuffer(GL_ARRAY_BUFFER, self->id);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(TW_Vector3) * (size == -1 ? self->size : size),
               &self->data[0], self->usage);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  engine_gl_check();
}

void TW_Vector2BufferInit(TW_Vector2Buffer *self, int size, GLenum u) {
  glGenBuffers(1, &self->id);
  engine_gl_check();
  self->size = size;
  self->usage = u;
  self->data = (TW_Vector2 *)malloc(sizeof(TW_Vector2) * size);
}

TW_Vector2Buffer *TW_Vector2BufferCreate(int size, GLenum u) {
  TW_Vector2Buffer *v = (TW_Vector2Buffer *)malloc(sizeof(TW_Vector2Buffer));
  TW_Vector2BufferInit(v, size, u);
  return v;
}

void TW_Vector2BufferDestroy(TW_Vector2Buffer *self) {
  glDeleteBuffers(1, &self->id);
  free(self->data);
  engine_gl_check();
}

void TW_Vector2BufferBind(TW_Vector2Buffer *self, int index) {

  glBindBuffer(GL_ARRAY_BUFFER, self->id);
  engine_gl_check();
  glEnableVertexAttribArray(index);
  engine_gl_check();
  glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, 0, 0);
  engine_gl_check();

  // glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
  // glEnableClientState(GL_point_buffer);
  // glVertexPointer(3, GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(0));    // The
  // starting TW_Vector2 of the VBO, for the vertices
}

void TW_Vector2BufferUpdate(TW_Vector2Buffer *self, int size) {
  glBindBuffer(GL_ARRAY_BUFFER, self->id);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(TW_Vector2) * (size == -1 ? self->size : size),
               &self->data[0], self->usage);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  engine_gl_check();
}
