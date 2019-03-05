#ifndef POINT_BUFFER_H
#define POINT_BUFFER_H

#include "../core/engine.h"
#include "../math/math.h"

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

void TW_Vector2BufferInit(TW_Vector2Buffer *self, int size, GLenum u);
TW_Vector2Buffer *TW_Vector2BufferCreate(int size, GLenum u);
void TW_Vector2BufferFree(TW_Vector2Buffer *self);
void TW_Vector2BufferBind(TW_Vector2Buffer *self, int index);
void TW_Vector2BufferUpdate(TW_Vector2Buffer *self, int size);

void TW_Vector3BufferInit(TW_Vector3Buffer *self, int size, GLenum u);
TW_Vector3Buffer *TW_Vector3BufferCreate(int size, GLenum u);
void TW_Vector3BufferFree(TW_Vector3Buffer *self);
void TW_Vector3BufferBind(TW_Vector3Buffer *self, int index);
void TW_Vector3BufferUpdate(TW_Vector3Buffer *self, int size);

#endif