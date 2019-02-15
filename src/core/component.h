#ifndef COMPONENT_H
#define COMPONENT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>

#include "../math/math.h"

struct component_t;

typedef void (*render_callback)(struct component_t *self);
typedef void (*click_callback)(struct component_t *self, TW_Point coords);
typedef int (*update_callback)(struct component_t *self);

typedef struct component_t_state {
} component_state;

struct texture_t;

typedef struct component_t {
#include "component_props.inc"
} TW_Component;

typedef struct window_t {
  SDL_Window *TW_Window;
  int height;
  int width;
  int passed;
  int frame_time;
  int frame;
} TW_Window;

TW_Component *component_create(TW_Component props);
TW_Component **component_list_create(int count, ...);

void component_destroy(TW_Component *self);
void component_render_children(TW_Component *self);
void component_update_pass(TW_Component *self);
TW_Component *component_at_point(TW_Component *self, TW_Point coords);

void component_move(TW_Component *self, int x, int y);
void component_resize(TW_Component *self, int w, int h);

#define COMPONENT_DEFAULTS                                                     \
  .TW_Window = WINDOW_DEFAULT, .children = 0, .update = 0, .cache = 0,         \
  .resized = 1, .rect = {0, 0, 0, 0}

#define _NUMARGS(type, ...) (sizeof((type[]){__VA_ARGS__}) / sizeof(type))

#define TW_Component(elem_type, ...)                                           \
  ({                                                                           \
    elem_type *e = malloc(sizeof(elem_type));                                  \
    elem_type initial = {COMPONENT_DEFAULTS, __VA_ARGS__};                     \
    memcpy(e, &initial, sizeof(initial));                                      \
    e;                                                                         \
  })

#define LIST(...)                                                              \
  component_list_create(_NUMARGS(TW_Component *, __VA_ARGS__), __VA_ARGS__)

#define CHILDREN(...) .children = LIST(__VA_ARGS__)

#endif