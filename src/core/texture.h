#ifndef TEXTURE_H
#define TEXTURE_H

#include "engine.h"

typedef struct texture_t {
  GLuint buffer;
  GLuint texture;
  int width;
  int height;
} texture;

texture *texture_render_target(int w, int h);
texture *texture_load(const char *filename);
void texture_destroy(texture *);
void texture_start_render(texture *t);
void texture_end_render(texture *t);

void texture_draw(texture *t, rectangle r);
#endif