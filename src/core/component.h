#ifndef COMPONENT_H
#define COMPONENT_H

#include <SDL2/SDL.h>

struct s_component;

typedef void (*render_callback)(struct s_component *self);
typedef void (*click_callback)(struct s_component *self, SDL_Point point);
typedef int (*update_callback)(struct s_component *self);

typedef struct s_component_state {
} component_state;

typedef struct s_component {
#include "component_props.inc"
} component;

typedef struct s_window {
  SDL_Window *window;
  SDL_Renderer *renderer;
  int passed;
  int frame_time;
  int frame;
} window;

component *component_create(component props);
component **component_list_create(int count, ...);

void component_destroy(component *self);
void component_render_children(component *self);
void component_update_pass(component *self);
component *component_at_point(component *self, SDL_Point point);

void component_move(component *self, int x, int y);
void component_resize(component *self, int w, int h);

#define COMPONENT_DEFAULTS                                                     \
  .window = WINDOW_DEFAULT, .children = 0, .update = 0, .texture = 0,          \
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