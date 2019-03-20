#ifndef TW_CORE_BUFFER_H
#define TW_CORE_BUFFER_H

#include "sdl.h"

#include "math.h"

typedef struct TW_Vector2Buffer_t {
  GLuint id;
  GLenum usage;
  int size;
  TW_Vector2 *data;
} TW_Vector2Buffer;

typedef struct TW_Vector3Buffer_t {

  GLuint id;
  GLuint vaid;
  GLenum usage;

  int size;
  TW_Vector3 *data;

} TW_Vector3Buffer;

typedef struct TW_IndicesBuffer_t {

  GLuint id;
  GLuint vaid;
  GLenum usage;

  int size;
  GLuint *data;

} TW_IndicesBuffer;

void TW_Vector2BufferInit(TW_Vector2Buffer *self, int size, GLenum u);
TW_Vector2Buffer *TW_Vector2BufferCreate(int size, GLenum u);
void TW_Vector2BufferRealloc(TW_Vector2Buffer *self, int size);
void TW_Vector2BufferFree(TW_Vector2Buffer *self);
void TW_Vector2BufferBind(TW_Vector2Buffer *self, int index);
void TW_Vector2BufferUpdate(TW_Vector2Buffer *self, int size);

void TW_Vector3BufferInit(TW_Vector3Buffer *self, int size, GLenum u);
TW_Vector3Buffer *TW_Vector3BufferCreate(int size, GLenum u);
void TW_Vector3BufferRealloc(TW_Vector3Buffer *self, int size);
void TW_Vector3BufferFree(TW_Vector3Buffer *self);
void TW_Vector3BufferBind(TW_Vector3Buffer *self, int index);
void TW_Vector3BufferUpdate(TW_Vector3Buffer *self, int size);

void TW_IndicesBufferInit(TW_IndicesBuffer *self, int size, GLenum u);
TW_IndicesBuffer *TW_IndicesBufferCreate(int size, GLenum u);
void TW_IndicesBufferRealloc(TW_IndicesBuffer *self, int size);
void TW_IndicesBufferFree(TW_IndicesBuffer *self);
void TW_IndicesBufferBind(TW_IndicesBuffer *self);
void TW_IndicesBufferUpdate(TW_IndicesBuffer *self, int size);

#endif