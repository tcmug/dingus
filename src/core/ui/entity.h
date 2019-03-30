#ifndef ENTITY_H
#define ENTITY_H

#include "../math.h"
#include "../sdl.h"

#include "utlist.h"

struct TW_Entity_t;

typedef void (*render_callback)(struct TW_Entity_t *self);
typedef void (*click_callback)(struct TW_Entity_t *self, TW_Vector2 coords);
typedef int (*update_callback)(struct TW_Entity_t *self);

extern int entity_id_counter;

#include "window.h"

typedef struct TW_Entity_t {
#include "entity_props.inc"
} TW_Entity;

TW_Entity *TW_EntityCreate(TW_Entity props);
void TW_EntityRerender(TW_Entity *);
TW_Entity *TW_GetRoot(TW_Entity *comp);

void TW_EntityFree(TW_Entity *self);
void TW_EntityRenderChildren(TW_Entity *self);
void TW_EntityUpdatePass(TW_Entity *self);
TW_Entity *TW_EntityAtPoint(TW_Entity *self, TW_Vector2 coords);

void TW_EntityMove(TW_Entity *self, int x, int y);
void TW_EntityResize(TW_Entity *self, int w, int h);
void TW_EntityAppendChild(TW_Entity *self, TW_Entity *child);
void TW_EntityPrependChild(TW_Entity *self, TW_Entity *child);
void TW_EntityRemoveChild(TW_Entity *self, TW_Entity *child);

#define ENTITY_DEFAULTS                                                        \
  .cache = 0, .resized = 1, .rect = {0, 0, 0, 0}, .parent = 0,                 \
  .render = TW_EntityRenderChildren, .shader = 0

#define _NUMARGS(type, ...) (sizeof((type[]){__VA_ARGS__}) / sizeof(type))

#define TW_Entity(elem_type, ...)                                              \
  ({                                                                           \
    elem_type *e = (elem_type *)malloc(sizeof(elem_type));                     \
    elem_type initial = {ENTITY_DEFAULTS, __VA_ARGS__};                        \
    e->id = entity_id_counter++;                                               \
    memcpy(e, &initial, sizeof(initial));                                      \
    e;                                                                         \
  })

#define ENTITY(...) ((TW_Entity *)TW_Entity(TW_Entity, __VA_ARGS__))

#endif