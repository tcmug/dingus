

#include <SDL2/SDL.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "component.h"
#include "log.h"

void component_render_children(component *self) {
  if (self->children)
    for (int i = 0; self->children[i]; i++)
      if (self->children[i]->render)
        self->children[i]->render(self->children[i]);
}

void component_update_pass(component *self) {
  if (self->update)
    self->update(self);
  if (self->children)
    for (int i = 0; self->children[i]; i++)
      component_update_pass(self->children[i]);
}

component *component_at_point(component *self, TW_Point coord) {
  // if (SDL_PointInRect(&TW_Point, &self->rect)) {
  //   if (self->children)
  //     for (int i = 0; self->children[i]; i++) {
  //       component *test = component_at_point(self->children[i], TW_Point);
  //       if (test)
  //         return test;
  //     }
  //   return self;
  // }
  return 0;
}

void component_move(component *self, int x, int y) {
  self->rect.x = x;
  self->rect.y = y;
}

void component_resize(component *self, int w, int h) {
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
