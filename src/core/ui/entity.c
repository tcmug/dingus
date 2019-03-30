

#include <SDL2/SDL.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../log.h"
#include "entity.h"

int entity_id_counter = 1;

void TW_EntityRenderChildren(TW_Entity *self) {
  TW_Entity *kid;
  DL_FOREACH(self->kids, kid) kid->render(kid);
}

void TW_EntityRerender(TW_Entity *self) {
  TW_Entity *comp = self;
  while (comp) {
    comp->rerender = 1;
    comp = comp->parent;
  }
}

TW_Entity *TW_EntityAtPoint(TW_Entity *self, TW_Vector2 coord) {
  TW_Entity *match = 0;
  printf("%f %f %f %f -- %f %f\n", self->rect.x, self->rect.y, self->rect.w,
         self->rect.h, coord.x, coord.y);
  if (TW_RectangleIncludesPoint(self->rect, coord)) {
    match = self;
    TW_Vector2 temp = {coord.x - self->rect.x, coord.y - self->rect.y};
    TW_Entity *kid;
    DL_FOREACH(self->kids, kid) {
      TW_Entity *test = TW_EntityAtPoint(kid, temp);
      if (test)
        match = test;
    }
  }
  return match;
}

void TW_EntityAppendChild(TW_Entity *self, TW_Entity *child) {
  DL_APPEND(self->kids, child);
  child->parent = self;
}

void TW_EntityPrependChild(TW_Entity *self, TW_Entity *child) {
  DL_PREPEND(self->kids, child);
  child->parent = self;
}

void TW_EntityRemoveChild(TW_Entity *self, TW_Entity *child) {
  DL_DELETE(self->kids, child);
  child->parent = 0;
}

void TW_EntityMove(TW_Entity *self, int x, int y) {
  self->rect.x = x;
  self->rect.y = y;
}

void TW_EntityResize(TW_Entity *self, int w, int h) {
  if (self->rect.w != w || self->rect.h != h) {
    self->resized = 1;
    self->rect.w = w;
    self->rect.h = h;
    TW_EntityRerender(self);
  }
}

void TW_EntityFree(TW_Entity *self) {
  TW_Entity *kid, *tmp;
  DL_FOREACH_SAFE(self->kids, kid, tmp) { TW_EntityFree(kid); }
  free(self);
}

TW_Entity *TW_GetRoot(TW_Entity *comp) {
  while (comp->parent) {
    comp = comp->parent;
  }
  return comp;
}