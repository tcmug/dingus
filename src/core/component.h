#ifndef COMPONENT_H
#define COMPONENT_H

#include <SDL2/SDL.h>

struct s_component;

typedef void (*render_callback)(struct s_component *self);
typedef void (*click_callback)(struct s_component *self);
typedef int (*update_callback)(struct s_component *self);

typedef struct s_component {
  const char *name;
  struct s_component **children;
  void *state;
  struct s_window *window;
  SDL_Rect rect;
  int resized;
  SDL_Texture *texture;
  update_callback update;
  render_callback render;
  click_callback click;
  // click
  // doubleclick
  // focus
  // blur
  // ...
} component;

typedef struct s_window {
  SDL_Window *window;
  SDL_Renderer *renderer;
  int passed;
  int frame;
} window;

component *_NODE(component props);
component **_LIST(int count, ...);
void DESTROY(component *self);
void node_render_children(component *self);

void node_move(component *self, int x, int y);
void node_resize(component *self, int w, int h);

#define COMP_DEFAULTS                                                          \
  .name = "NO-NAME", .children = 0, .update = 0, .texture = 0, .resized = 1
#define NUMARGS(type, ...) (sizeof((type[]){__VA_ARGS__}) / sizeof(type))

#define NODE(...) _NODE((component){COMP_DEFAULTS, __VA_ARGS__})
#define LIST(...) _LIST(NUMARGS(component *, __VA_ARGS__), __VA_ARGS__)

#endif