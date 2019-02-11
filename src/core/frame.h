#ifndef FRAME_H
#define FRAME_H

#include "engine.h"

typedef struct s_frame {
  GLuint buffer;
  GLuint texture;
  int width;
  int height;
} frame;

frame *frame_create();
void frame_destroy(frame *frame);

void frame_bind(frame *f);
void frame_unbind(frame *f);

#endif