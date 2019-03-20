#ifndef TEXTURE_H
#define TEXTURE_H

#include "engine.h"

typedef struct TW_Texture_t {
  GLuint buffer;
  GLuint texture;
  GLuint depth;
  int width;
  int height;
  GLint previous_viewport[4];
  GLint previous_buffer;
} TW_Texture;

TW_Texture *TW_TextureRenderTarget(int w, int h, int hasdepth);
TW_Texture *TW_TextureLoad(const char *filename);
void TW_TextureFree(TW_Texture *);
void TW_TextureStartRender(TW_Texture *t);
void TW_TextureEndRender(TW_Texture *t);
void TW_TextureDraw(TW_Texture *t, TW_Rectangle r);

#endif