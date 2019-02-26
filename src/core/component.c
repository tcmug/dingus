

#include <SDL2/SDL.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "component.h"
#include "log.h"

void TW_ComponentRenderChildren(TW_Component *self) {
  TW_Component *kid;
  DL_FOREACH(self->kids, kid)
  if (kid->render) {
    kid->render(self, kid);
  }
}

void TW_ComponentRerender(TW_Component *self) {
  TW_Component *c = self;
  while (c) {
    c->rerender = 1;
    c = c->parent;
  }
}

// void TW_ComponentUpdatePass(TW_Component *self) {
//   if (self->update)
//     self->update(self);
//   if (self->children)
//     for (int i = 0; self->children[i]; i++)
//       TW_ComponentUpdatePass(self->children[i]);
// }

TW_Component *TW_ComponentAtPoint(TW_Component *self, TW_Vector2 coord) {
  app_log("%p", self);
  TW_Component *match = 0;
  if (TW_RectangleIncludesPoint(self->rect, coord)) {
    match = self;
    if (self->children) {
      TW_Vector2 temp = {coord.x - self->rect.x, coord.y - self->rect.y};
      for (int i = 0; self->children[i]; i++) {
        TW_Component *test = TW_ComponentAtPoint(self->children[i], temp);
        if (test)
          match = test;
      }
    }
  }
  return match;
}

void TW_ComponentMove(TW_Component *self, int x, int y) {
  self->rect.x = x;
  self->rect.y = y;
}

void TW_ComponentResize(TW_Component *self, int w, int h) {
  if (self->rect.w != w || self->rect.h != h) {
    self->resized = 1;
    self->rect.w = w;
    self->rect.h = h;
    TW_ComponentRerender(self);
  }
}

TW_Component **TW_ComponentListCreate(int count, ...) {
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

void TW_ComponentDestroy(TW_Component *self) {
  if (self->children) {
    int i = 0;
    while (self->children[i]) {
      TW_ComponentDestroy(self->children[i++]);
    }
  }
  free(self->children);
  free(self);
}
