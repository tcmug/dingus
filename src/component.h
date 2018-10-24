#ifndef COMPONENT_H
#define COMPONENT_H

#include <SDL2/SDL.h>

struct s_component;

typedef void (*render_callback)(struct s_component *this);
typedef void (*click_callback)(struct s_component *this);

typedef struct s_component {
  const char *name;
  struct s_component **children;
  void *state;
  void *props;
  SDL_Rect rect;
  render_callback render;
  click_callback click;
  // click
  // doubleclick
  // focus
  // blur
  // ...
} component;

component *_NODE(component props);
component **_LIST(int count, ...);
void DESTROY(component *this);

#define COMP_DEFAULTS .name = "NO-NAME", .children = 0
#define NUMARGS(type, ...) (sizeof((type[]){__VA_ARGS__}) / sizeof(type))

#define NODE(...) _NODE((component){COMP_DEFAULTS, __VA_ARGS__})
#define LIST(...) _LIST(NUMARGS(component *, __VA_ARGS__), __VA_ARGS__)

#endif