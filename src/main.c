
#include <SDL2/SDL.h>

#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

#include "config.h"

#include "core/engine.h"
#include "core/log.h"
#include "core/math.h"
#include "core/ui.h"

#include "fun/scene.h"

#define APP_LOOP_EVENT_BLOCKS_RENDER

TW_Window *window = 0;

#include "utlist.h"

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

  if (luaL_dofile(window->lua, RESOURCE("share/dingus/scripts/start.lua"))) {
    app_log("Couldn't load file: %s\n", lua_tostring(window->lua, -1));
    return 0;
  }

  lua_getglobal(window->lua, "screen");
  TW_Entity *root = (TW_Entity *)lua_touserdata(window->lua, -1);
  if (!root) {
    app_log("No root element for screen.");
    return 1;
  }

  TW_EntityAppendChild((TW_Entity *)window, root);

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
          TW_EntityResize(root, window->width, window->height);
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
        printf("%i %i\n", coord.x, coord.y);
        TW_Entity *self = TW_EntityAtPoint(root, coord);
        if (self) {
          printf("%p\n", self);
          if (self->click) {
            self->click(self, coord);
          }
          if (self->lua_click) {
            lua_get_callback_method(window->lua, self->lua_click);
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
      // fps_display->update((TW_Entity *)fps_display);
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
