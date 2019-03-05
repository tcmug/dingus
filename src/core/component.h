#ifndef COMPONENT_H
#define COMPONENT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>

#include "../math/math.h"

#include "utlist.h"

struct component_t;

typedef void (*render_callback)(const struct component_t *parent,
                                struct component_t *self);
typedef void (*click_callback)(struct component_t *self, TW_Vector2 coords);
typedef int (*update_callback)(struct component_t *self);

typedef struct component_t_state {
} component_state;

struct texture_t;

typedef struct component_t {
#include "component_props.inc"
} TW_Component;

typedef struct window_t {

  SDL_Window *sdl_window;
  struct lua_State *lua;

  int height;
  int width;
  int resizable;
  int vsync;
  int high_dpi;
  int frame_limit;
  int fullscreen;

  int passed;
  int frame_time;
  int frame;

} TW_Window;

TW_Component *TW_ComponentCreate(TW_Component props);
TW_Component **TW_ComponentListCreate(int count, ...);
void TW_ComponentRerender(TW_Component *);

void TW_ComponentFree(TW_Component *self);
void TW_ComponentRenderChildren(TW_Component *self);
void TW_ComponentUpdatePass(TW_Component *self);
TW_Component *TW_ComponentAtPoint(TW_Component *self, TW_Vector2 coords);

void TW_ComponentMove(TW_Component *self, int x, int y);
void TW_ComponentResize(TW_Component *self, int w, int h);
void TW_ComponentAppendChild(TW_Component *self, TW_Component *child);
void TW_ComponentPrependChild(TW_Component *self, TW_Component *child);
void TW_ComponentRemoveChild(TW_Component *self, TW_Component *child);

#define COMPONENT_DEFAULTS                                                     \
  .window = WINDOW_DEFAULT, .cache = 0, .resized = 1, .rect = {0, 0, 0, 0},    \
  .parent = 0

#define _NUMARGS(type, ...) (sizeof((type[]){__VA_ARGS__}) / sizeof(type))

#define TW_Component(elem_type, ...)                                           \
  ({                                                                           \
    elem_type *e = (elem_type *)malloc(sizeof(elem_type));                     \
    elem_type initial = {COMPONENT_DEFAULTS, __VA_ARGS__};                     \
    memcpy(e, &initial, sizeof(initial));                                      \
    e;                                                                         \
  })

#define LIST(...)                                                              \
  TW_ComponentListCreate(_NUMARGS(TW_Component *, __VA_ARGS__), __VA_ARGS__)

#define CHILDREN(...) .children = LIST(__VA_ARGS__)

#define COMPONENT(...) ((TW_Component *)TW_Component(TW_Component, __VA_ARGS__))

#endif