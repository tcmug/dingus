

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

TW_Component *TW_ComponentAtPoint(TW_Component *self, TW_Vector2 coord) {
  TW_Component *match = 0;
  if (TW_RectangleIncludesPoint(self->rect, coord)) {
    match = self;
    TW_Vector2 temp = {coord.x - self->rect.x, coord.y - self->rect.y};
    TW_Component *kid;
    DL_FOREACH(self->kids, kid) {
      TW_Component *test = TW_ComponentAtPoint(kid, temp);
      if (test)
        match = test;
    }
  }
  return match;
}

void TW_ComponentAppendChild(TW_Component *self, TW_Component *child) {
  DL_APPEND(self->kids, child);
  child->parent = self;
}

void TW_ComponentPrependChild(TW_Component *self, TW_Component *child) {
  DL_PREPEND(self->kids, child);
  child->parent = self;
}

void TW_ComponentRemoveChild(TW_Component *self, TW_Component *child) {
  DL_DELETE(self->kids, child);
  child->parent = 0;
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

void TW_ComponentFree(TW_Component *self) {
  TW_Component *kid, *tmp;
  DL_FOREACH_SAFE(self->kids, kid, tmp) { TW_ComponentFree(kid); }
  free(self);
}
