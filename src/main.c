
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
#include "core/component.h"
#include "core/engine.h"
#include "core/log.h"
#include "core/print.h"

#include "math/math.h"

#include "core/buffer.h"
#include "core/shader.h"
#include "core/texture.h"

#include "fun/body/body.h"

#define APP_LOOP_EVENT_BLOCKS_RENDER

TW_Window *props = 0;

#define WINDOW_DEFAULT (props)

TW_Vector3Buffer va;
TW_Shader *flat, *uishader;

bodysphere sphere;
TW_Texture *spheret;

void PolyRender(const TW_Component *parent, TW_Component *self) {

  real ratio = parent->rect.w / parent->rect.h;
  TW_Matrix projection = TW_MatrixPerspectiveProjection(0.01, 1000, 70, ratio);
  TW_Matrix view = TW_MatrixFromVector(
      (TW_Vector3){0, 0, 1.4}, (TW_Vector3){0, 0, 0}, (TW_Vector3){0, 1, 0});

  static float a = 0;
  a += 0.0001;
  TW_Matrix model = TW_MatrixRotation(a, a, 0);

  TW_ShaderUse(flat);

  TW_MatrixGLUniform("projection", projection);
  TW_MatrixGLUniform("view", view);
  TW_MatrixGLUniform("model", model);

  TW_Vector3BufferBind(&va, 0);
  // glDrawArrays(GL_TRIANGLES, 0, va.size);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, spheret->texture);

  bodysphere_update(&sphere, (TW_Vector3){0, 0, 2});
  sphere.position.x = -1;
  bodysphere_render(&sphere);

  TW_ShaderUse(uishader);
  TW_ComponentRerender(self);
}

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

static int l_view(lua_State *L) {
  const char *type = lua_get_table_string(L, "type", "null");
  int x = lua_get_table_int(L, "x", 0);
  int y = lua_get_table_int(L, "y", 0);
  int w = lua_get_table_int(L, "w", props->width);
  int h = lua_get_table_int(L, "h", props->height);

  TW_Component *comp = VIEW(.color = {0, 0, 0, 0}, .rect = {x, y, w, h});

  l_prep_kids(L, comp);
  lua_pushlightuserdata(L, comp);
  return 1;
}

static int l_text(lua_State *L) {
  const char *type = lua_get_table_string(L, "type", "null");

  int x = lua_get_table_int(L, "x", 0);
  int y = lua_get_table_int(L, "y", 0);
  int w = lua_get_table_int(L, "w", props->width);
  int h = lua_get_table_int(L, "h", props->height);

  const char *cstr = lua_get_table_string(L, "text", "null");

  TW_Component *comp = TEXT(.rect = {x, y, w, h},
                            .color =
                                {
                                    1,
                                    1,
                                    1,
                                },
                            .text = cstr);

  l_prep_kids(L, comp);
  lua_pushlightuserdata(L, comp);
  return 1;
}

// elem_type
int main(int argc, char *args[]) {

  props = engine_init();
  if (!props)
    return 1;

  int running = 1;
  int fps = 0, real_fps = 0, fps_counter = 1000;
  int previous = SDL_GetTicks();

  wchar_t fps_display_string[0xFF] = {0};

  int fullscreen = 0;

  srand(SDL_GetTicks());

  flat = TW_ShaderLoad(RESOURCE("share/dingus/shaders/flat.vert"), 0,
                       RESOURCE("share/dingus/shaders/flat.frag"));

  uishader = TW_ShaderLoad(RESOURCE("share/dingus/shaders/ui.vert"), 0,
                           RESOURCE("share/dingus/shaders/ui.frag"));

  default_font =
      TW_FontLoad(RESOURCE("share/dingus/NotoSerif-Regular.ttf"), 24);

  // GL_STATIC_DRAW
  TW_Vector3BufferInit(&va, 9, GL_STREAM_DRAW);
  TW_Vector3 offset = {0};

  offset.x = -1;
  offset.y = 1;
  va.data[0] = TW_Vector3SubVector((TW_Vector3){-1, -1, 0}, offset);
  va.data[1] = TW_Vector3SubVector((TW_Vector3){0, 1, 0}, offset);
  va.data[2] = TW_Vector3SubVector((TW_Vector3){1, -1, 0}, offset);

  offset.x = 1;
  offset.y = 1;
  va.data[3] = TW_Vector3SubVector((TW_Vector3){-1, -1, 0}, offset);
  va.data[4] = TW_Vector3SubVector((TW_Vector3){0, 1, 0}, offset);
  va.data[5] = TW_Vector3SubVector((TW_Vector3){1, -1, 0}, offset);

  offset.x = 0;
  offset.y = -1;
  va.data[6] = TW_Vector3SubVector((TW_Vector3){-1, -1, 0}, offset);
  va.data[7] = TW_Vector3SubVector((TW_Vector3){0, 1, 0}, offset);
  va.data[8] = TW_Vector3SubVector((TW_Vector3){1, -1, 0}, offset);

  TW_Vector3BufferUpdate(&va, va.size);

  lua_pushcfunction(props->lua, l_view);
  lua_setglobal(props->lua, "VIEW");

  lua_pushcfunction(props->lua, l_text);
  lua_setglobal(props->lua, "TEXT");

  if (luaL_dofile(props->lua, RESOURCE("share/dingus/scripts/start.lua"))) {
    app_log("Couldn't load file: %s\n", lua_tostring(props->lua, -1));
    return 0;
  }

  lua_getglobal(props->lua, "screen");
  TW_Component *root = (TW_Component *)lua_touserdata(props->lua, -1);
  if (!root) {
    app_log("No root element for screen.");
    return 1;
  }

  TW_Component *screen = VIEW(.rect = {0, 0, props->width, props->height},
                              .color = {0, 0, 0, 0}, .hasDepth = 1);
  TW_Component *threed = COMPONENT(.render = PolyRender);

  TW_ComponentAppendChild(screen, threed);
  TW_ComponentPrependChild(root, screen);

  spheret = TW_TextureLoad("dirt.jpg");
  bodysphere_create(&sphere, 1);

  while (running) {
    SDL_Event ev;
#ifdef APP_LOOP_EVENT_BLOCKS_RENDER
    if (SDL_PollEvent(&ev)) {
#else
    if (SDL_WaitEvent(&ev)) {
#endif
      switch (ev.type) {
      case SDL_WINDOWEVENT: {
        switch (ev.window.event) {
        case SDL_WINDOWEVENT_SIZE_CHANGED:
          props->width = ev.window.data1;
          props->height = ev.window.data2;
          TW_ComponentResize(root, props->width, props->height);
          break;
        }
      } break;
      case SDL_QUIT:
        running = 0;
        break;
      case SDL_MOUSEBUTTONDOWN: {
        TW_Vector2 coord = {ev.motion.x, props->height - ev.motion.y};
        TW_Component *self = TW_ComponentAtPoint(root, coord);
        app_log("TW_Component at TW_Vector2 (%f, %f) is %p", coord.x, coord.y,
                self);
        if (self && self->click) {
          self->click(self, coord);
        }
      } break;
      case SDL_KEYUP:
        switch (ev.key.keysym.sym) {
        case SDLK_ESCAPE:
          running = 0;
          break;
        case SDLK_f:
          fullscreen = !fullscreen;
          if (fullscreen) {
            app_log("Enter fullscreen mode.");
            SDL_SetWindowFullscreen(props->sdl_window, SDL_WINDOW_FULLSCREEN);
          } else {
            app_log("Exit fullscreen mode.");
            SDL_SetWindowFullscreen(props->sdl_window, 0);
          }
        }
      }
    }

    if (root->rerender) {

      const int start = SDL_GetTicks();
      const int frame_time = (start - previous);
      props->frame_time = frame_time;
      props->passed += frame_time;
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

      // glViewport(0, 0, props->width, props->height);
      // glClearColor(0.2, 0, 0, 0);
      // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
      //         GL_STENCIL_BUFFER_BIT);
      // engine_gl_check();

      // GLint program, loc, u;

      // static GLint tint_loc = -1;
      // if (tint_loc == -1) {
      //   tint_loc = TW_ShaderGLUniformLoc(uishader, "tint");
      // }
      // TW_Vector3GLUniformLoc(tint_loc, (TW_Vector3){1, 1, 1});
      // glGetIntegerv(GL_CURRENT_PROGRAM, &program);
      // u = glGetUniformLocation(program, "surface");
      // glUniform1i(u, 0);

      TW_ShaderUse(uishader);
      root->render(0, root);
      SDL_GL_SwapWindow(props->sdl_window);

      // FPS limit.
      const int end = SDL_GetTicks();
      if (props->frame_limit) {
        const int to_delay = ((1000 / props->frame_limit) - (end - start));
        if (to_delay > 0) {
          SDL_Delay(to_delay);
        }
      }
    }
  }

  TW_ShaderFree(flat);
  TW_ShaderFree(uishader);
  return engine_shutdown(props);
}
