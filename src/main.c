
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

#include "config.h"

#include "components/center.h"
#include "components/right.h"
#include "components/text.h"
#include "components/top.h"
#include "components/view.h"

#include "core/engine.h"
#include "core/log.h"
#include "core/math.h"
#include "core/ui.h"

#include "fun/scene.h"

#define APP_LOOP_EVENT_BLOCKS_RENDER

TW_Window *window = 0;

#define WINDOW_DEFAULT (window)
#include "utlist.h"

void l_prep_kids(lua_State *L, TW_Component *comp) {
  comp->kids = 0;
  lua_pushstring(L, "children");
  lua_gettable(L, -2);
  if (lua_istable(L, -1)) {
    lua_pushnil(L);
    while (lua_next(L, -2) != 0) {
      if (lua_islightuserdata(L, -1)) {
        TW_ComponentAppendChild(comp, (TW_Component *)lua_touserdata(L, -1));
      }
      lua_pop(L, 1);
    }
  }
  lua_pop(L, 1);
}

static int l_VIEW(lua_State *L) {
  int x = lua_get_table_int(L, "x", 0);
  int y = lua_get_table_int(L, "y", 0);
  int w = lua_get_table_int(L, "w", window->width);
  int h = lua_get_table_int(L, "h", window->height);
  int depth = lua_get_table_int(L, "depth", 0);
  app_log("VIEW(%i,%i %i,%i %u)", x, y, h, w, depth);
  TW_Component *comp =
      VIEW(.color = {0, 0, 0, 0}, .rect = {x, y, w, h}, .hasDepth = depth);
  l_prep_kids(L, comp);
  lua_pushlightuserdata(L, comp);
  return 1;
}

static int l_parent(lua_State *L) {
  TW_Component *comp = (TW_Component *)lua_touserdata(L, -1);
  lua_pushlightuserdata(L, comp->parent);
  return 1;
}

static int l_insert(lua_State *L) {
  if (lua_isuserdata(L, -2)) {
    TW_Component *comp = lua_touserdata(L, -2);
    TW_ComponentAppendChild(comp, (TW_Component *)lua_touserdata(L, -1));
    TW_ComponentRerender(comp);
    lua_pushlightuserdata(L, comp->parent);
  } else {
    app_log("not user data");
  }
  return 0;
}

static int l_COMPONENT(lua_State *L) {

  int x = lua_get_table_int(L, "x", 0);
  int y = lua_get_table_int(L, "y", 0);
  int w = lua_get_table_int(L, "w", window->width);
  int h = lua_get_table_int(L, "h", window->height);

  app_log("COMPONENT(%i,%i %i,%i)", x, y, h, w);
  TW_Component *comp = COMPONENT(.rect = {x, y, w, h});

  l_prep_kids(L, comp);
  lua_pushlightuserdata(L, comp);
  return 1;
}

static int l_quit(lua_State *L) {
  window->running = 0;
  return 0;
}

int callback_index = -1;

static int l_text(lua_State *L) {
  int x = lua_get_table_int(L, "x", 0);
  int y = lua_get_table_int(L, "y", 0);
  int w = lua_get_table_int(L, "w", window->width);
  int h = lua_get_table_int(L, "h", window->height);

  app_log("TEXT(%i,%i %i,%i)", x, y, h, w);

  int lua_click = 0;
  lua_pushstring(L, "click");
  lua_gettable(L, -2);
  if (lua_isfunction(L, -1)) {
    if (callback_index == -1) {
      lua_newtable(L); // create table for functions
      callback_index = luaL_ref(L, LUA_REGISTRYINDEX);
    }
    lua_rawgeti(L, LUA_REGISTRYINDEX, callback_index);
    lua_pushvalue(L, -2);
    lua_click = luaL_ref(L, -2);
    lua_pop(L, 1);
  }
  lua_pop(L, 1);

  const char *cstr = lua_get_table_string(L, "text", "null");

  TW_Component *comp = TEXT(.rect = {x, y, w, h},
                            .color =
                                {
                                    1,
                                    1,
                                    1,
                                },
                            .text = cstr, .lua_click = lua_click);

  l_prep_kids(L, comp);
  lua_pushlightuserdata(L, comp);
  return 1;
}

// elem_type
int main(int argc, char *args[]) {

  window = engine_init();
  if (!window)
    return 1;

  int fps = 0, real_fps = 0, fps_counter = 1000;
  int previous = SDL_GetTicks();

  wchar_t fps_display_string[0xFF] = {0};

  int fullscreen = 0;

  srand(SDL_GetTicks());

  window->shader = TW_ShaderLoad(RESOURCE("share/dingus/shaders/ui.vert"), 0,
                                 RESOURCE("share/dingus/shaders/ui.frag"));

  default_font =
      TW_FontLoad(RESOURCE("share/dingus/NotoSerif-Regular.ttf"), 24);

  lua_pushcfunction(window->lua, l_VIEW);
  lua_setglobal(window->lua, "VIEW");

  lua_pushcfunction(window->lua, l_text);
  lua_setglobal(window->lua, "TEXT");

  lua_pushcfunction(window->lua, l_insert);
  lua_setglobal(window->lua, "insert");

  lua_pushcfunction(window->lua, l_parent);
  lua_setglobal(window->lua, "parent");

  lua_pushcfunction(window->lua, l_quit);
  lua_setglobal(window->lua, "quit");

  lua_pushcfunction(window->lua, l_COMPONENT);
  lua_setglobal(window->lua, "COMPONENT");

  lua_pushcfunction(window->lua, l_SCENE);
  lua_setglobal(window->lua, "SCENE");

  lua_pushcfunction(window->lua, l_SHADER);
  lua_setglobal(window->lua, "SHADER");

  lua_pushcfunction(window->lua, l_MODEL);
  lua_setglobal(window->lua, "MODEL");

  lua_pushcfunction(window->lua, l_OBJECT);
  lua_setglobal(window->lua, "OBJECT");

  if (luaL_dofile(window->lua, RESOURCE("share/dingus/scripts/start.lua"))) {
    app_log("Couldn't load file: %s\n", lua_tostring(window->lua, -1));
    return 0;
  }

  lua_getglobal(window->lua, "screen");
  TW_Component *root = (TW_Component *)lua_touserdata(window->lua, -1);
  if (!root) {
    app_log("No root element for screen.");
    return 1;
  }

  TW_ComponentAppendChild((TW_Component *)window, root);

  while (window->running) {
    SDL_Event ev;
#ifdef APP_LOOP_EVENT_BLOCKS_RENDER
    while (SDL_PollEvent(&ev)) {
#else
    if (SDL_WaitEvent(&ev)) {
#endif
      switch (ev.type) {

      case SDL_WINDOWEVENT: {
        switch (ev.window.event) {
        case SDL_WINDOWEVENT_SIZE_CHANGED:
          window->width = ev.window.data1;
          window->height = ev.window.data2;
          TW_ComponentResize(root, window->width, window->height);
          break;
        }
      } break;

      case SDL_MOUSEMOTION: {
        engine_report_cursor_position(window, &ev);
      } break;

      case SDL_QUIT:
        window->running = 0;
        break;

      case SDL_MOUSEBUTTONDOWN: {
        TW_Vector2 coord = {ev.motion.x, window->height - ev.motion.y + 1};
        TW_Component *self = TW_ComponentAtPoint(root, coord);
        /*app_log("TW_Component at (%f, %f) is %p (%f,%f %f,%f)", coord.x,
                coord.y, self, self->rect.x, self->rect.y, self->rect.w,
                self->rect.h);*/
        if (self) {
          if (self->click) {
            self->click(self, coord);
          }
          if (self->lua_click) {
            lua_rawgeti(window->lua, LUA_REGISTRYINDEX, callback_index);
            lua_rawgeti(window->lua, -1, self->lua_click);
            lua_pushlightuserdata(window->lua, self);
            lua_call(window->lua, 1, 0);
            lua_pop(window->lua, 1);
            app_log("%i", lua_gettop(window->lua));
          }
        }
      } break;
      case SDL_KEYUP:
        switch (ev.key.keysym.sym) {
        case SDLK_SPACE:
          if (window->cursor_mode == CURSOR_POINTER)
            engine_set_cursor_mode(window, CURSOR_VECTOR);
          else
            engine_set_cursor_mode(window, CURSOR_POINTER);
          break;
        case SDLK_ESCAPE:
          window->running = 0;
          break;
        case SDLK_r:
          TW_ShaderReloadAll();
          break;
        case SDLK_f:
          fullscreen = !fullscreen;
          if (fullscreen) {
            app_log("Enter fullscreen mode.");
            SDL_SetWindowFullscreen(window->sdl_window, SDL_WINDOW_FULLSCREEN);
          } else {
            app_log("Exit fullscreen mode.");
            SDL_SetWindowFullscreen(window->sdl_window, 0);
          }
        }
      }
    }

    const int start = SDL_GetTicks();
    const int frame_time = (start - previous);
    window->frame_time = frame_time;
    window->passed += frame_time;
    previous = start;

    fps_counter += frame_time;
    if (fps_counter >= 1000) {
      real_fps = fps;
      fps = 1;
      fps_counter -= 1000;
      app_log("FPS: %u", real_fps);
      swprintf(fps_display_string, 0xFF, L"FPS: %u", real_fps);
    } else {
      // fps_display->update((TW_Component *)fps_display);
      fps++;
    }

    TW_WindowRender(window);
    SDL_GL_SwapWindow(window->sdl_window);

    // FPS limit.
    const int end = SDL_GetTicks();
    if (window->frame_limit) {
      const int to_delay = ((1000 / window->frame_limit) - (end - start));
      if (to_delay > 0) {
        SDL_Delay(to_delay);
      }
    }
  }

  TW_ShaderFree(window->shader);
  return engine_shutdown(window);
}
