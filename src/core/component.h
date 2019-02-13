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
} component;

typedef struct window_t {
  SDL_Window *window;
  int height;
  int width;
  int passed;
  int frame_time;
  int frame;
} window;

component *component_create(component props);
component **component_list_create(int count, ...);

void component_destroy(component *self);
void component_render_children(component *self);
void component_update_pass(component *self);
component *component_at_point(component *self, TW_Point coords);

void component_move(component *self, int x, int y);
void component_resize(component *self, int w, int h);

#define COMPONENT_DEFAULTS                                                     \
  .window = WINDOW_DEFAULT, .children = 0, .update = 0, .cache = 0,            \
  .resized = 1, .rect = {0, 0, 0, 0}

#define _NUMARGS(type, ...) (sizeof((type[]){__VA_ARGS__}) / sizeof(type))

#define COMPONENT(elem_type, ...)                                              \
  ({                                                                           \
    elem_type *e = malloc(sizeof(elem_type));                                  \
    elem_type initial = {COMPONENT_DEFAULTS, __VA_ARGS__};                     \
    memcpy(e, &initial, sizeof(initial));                                      \
    e;                                                                         \
  })

#define LIST(...)                                                              \
  component_list_create(_NUMARGS(component *, __VA_ARGS__), __VA_ARGS__)

#define CHILDREN(...) .children = LIST(__VA_ARGS__)

#endif