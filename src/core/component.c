

#include <SDL2/SDL.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "component.h"

component *component_create(component init) {
  component *self = (component *)malloc(sizeof(component));
  memcpy(self, &init, sizeof(component));
  return self;
}

void node_render_children(component *self) {
  if (self->children)
    for (int i = 0; self->children[i]; i++)
      if (self->children[i]->render)
        self->children[i]->render(self->children[i]);
}

void node_move(component *self, int x, int y) {
  self->rect.x = x;
  self->rect.y = y; 
}

void node_resize(component *self, int w, int h) {
  self->resized = 1;
  self->rect.w = w;
  self->rect.h = h; 
}

component **component_list_create(int count, ...) {
  int i = 0;
  va_list args;
  va_start(args, count);
  component **children =
      (component **)malloc((count + 1) * sizeof(component *));
  while (i < count) {
    children[i++] = va_arg(args, component *);
  }
  children[i] = 0;
  va_end(args);
  return children;
}

void component_destroy(component *self) {
  if (self->children) {
    int i = 0;
    while (self->children[i]) {
      component_destroy(self->children[i++]);
    }
  }
  free(self->children);
  free(self);
}
