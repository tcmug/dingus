#ifndef WINDOW_H
#define WINDOW_H

#include "component.h"

typedef struct window_t {
#include "component_props.inc"

  SDL_Window *sdl_window;
  struct lua_State *lua;

  int running;
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

void TW_WindowRender(TW_Window *window);

#endif