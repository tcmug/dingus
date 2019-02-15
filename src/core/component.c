

#include <SDL2/SDL.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "component.h"
#include "log.h"

void component_render_children(TW_Component *self) {
  if (self->children)
    for (int i = 0; self->children[i]; i++)
      if (self->children[i]->render)
        self->children[i]->render(self->children[i]);
}

void component_update_pass(TW_Component *self) {
  if (self->update)
    self->update(self);
  if (self->children)
    for (int i = 0; self->children[i]; i++)
      component_update_pass(self->children[i]);
}

TW_Component *component_at_point(TW_Component *self, TW_Point coord) {
  if (TW_RectangleIncludesPoint(self->rect, coord)) {
    if (self->children)
      for (int i = 0; self->children[i]; i++) {
        TW_Component *test = component_at_point(self->children[i], coord);
        if (test)
          return test;
      }
    return self;
  }
  return 0;
}

void component_move(TW_Component *self, int x, int y) {
  self->rect.x = x;
  self->rect.y = y;
}

void component_resize(TW_Component *self, int w, int h) {
  self->resized = 1;
  self->rect.w = w;
  self->rect.h = h;
}

TW_Component **component_list_create(int count, ...) {
  int i = 0;
  va_list args;
  va_start(args, count);
  TW_Component **children =
      (TW_Component **)malloc((count + 1) * sizeof(TW_Component *));
  while (i < count) {
    children[i++] = va_arg(args, TW_Component *);
  }
  children[i] = 0;
  va_end(args);
  return children;
}

void component_destroy(TW_Component *self) {
  if (self->children) {
    int i = 0;
    while (self->children[i]) {
      component_destroy(self->children[i++]);
    }
  }
  free(self->children);
  free(self);
}
