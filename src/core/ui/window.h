#ifndef WINDOW_H
#define WINDOW_H

#include "component.h"

typedef enum { CURSOR_POINTER, CURSOR_VECTOR } TW_CursorMode;

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

  TW_CursorMode cursor_mode;
  TW_Vector2 cursor_vector;
  TW_Vector2 cursor_position;

  int passed;
  int frame_time;
  int frame;

} TW_Window;

void TW_WindowRender(TW_Window *window);

#endif