#ifndef TEXTURE_H
#define TEXTURE_H

#include "engine.h"

typedef struct texture_t {
  GLuint buffer;
  GLuint TW_Texture;
  int width;
  int height;
} TW_Texture;

TW_Texture *texture_render_target(int w, int h);
TW_Texture *texture_load(const char *filename);
void texture_destroy(TW_Texture *);
void texture_start_render(TW_Texture *t);
void texture_end_render(TW_Texture *t);

void texture_draw(TW_Texture *t, TW_Rectangle r);
#endif