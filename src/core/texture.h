#ifndef TEXTURE_H
#define TEXTURE_H

#include "engine.h"

typedef struct texture_t {
  GLuint buffer;
  GLuint TW_Texture;
  int width;
  int height;
} TW_Texture;

TW_Texture *TW_TextureRenderTarget(int w, int h);
TW_Texture *TW_TextureLoad(const char *filename);
void TW_TextureDestroy(TW_Texture *);
void TW_TextureStartRender(TW_Texture *t);
void TW_TextureEndRender(TW_Texture *t);
void TW_TextureDraw(TW_Texture *t, TW_Rectangle r);

#endif