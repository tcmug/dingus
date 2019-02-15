
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "config.h"

#include "components/center.h"
#include "components/right.h"
#include "components/text.h"
#include "components/view.h"
#include "core/component.h"
#include "core/engine.h"
#include "core/log.h"
#include "core/print.h"

#include "math/math.h"

#include "core/shader.h"

#include "core/texture.h"

#include "core/buffer.h"

TW_VectorBuffer va;
TW_Shader flat, uishader;

int mainUpdate(TW_Component *_self) {

  TW_TextureStartRender(_self->cache);

  glClearColor(0.2, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);
  engine_gl_check();

  TW_Vector dir = (TW_Vector){1, 1, -5};
  TW_Matrix projection = TW_MatrixPerspectiveProjection(0.01, 1000, 70, 1);
  TW_Matrix view = TW_MatrixFromVector(
      (TW_Vector){0, 0, 5}, (TW_Vector){0, 0, 0}, (TW_Vector){0, 1, 0});

  static float a = 0;
  a += 0.01;
  TW_Matrix model = TW_MatrixRotation(a, a, 0);

  TW_ShaderUse(flat);
  TW_MatrixGLUniform("projection", projection);
  TW_MatrixGLUniform("view", view);
  TW_MatrixGLUniform("model", model);

  TW_VectorBufferBind(&va, 0);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  TW_TextureEndRender(_self->cache);

  return 1;
}

// elem_type
int main(int argc, char *args[]) {

  TW_Window props = engine_init();

  int running = 1;
  int fps_limit = 60, fps = 60, real_fps = 0, fps_counter = 1000;
  int previous = SDL_GetTicks();

  wchar_t fps_display_string[0xFF] = {0};
  TW_Component *fps_display;

  int fullscreen = 0;

  srand(SDL_GetTicks());

  flat = TW_ShaderLoad(RESOURCE("share/dingus/shaders/flat.vert"), 0,
                       RESOURCE("share/dingus/shaders/flat.frag"));

  uishader = TW_ShaderLoad(RESOURCE("share/dingus/shaders/ui.vert"), 0,
                           RESOURCE("share/dingus/shaders/ui.frag"));

  default_font = font_atlas_create(RESOURCE("share/dingus/DroidSans.ttf"), 20);

  // GL_STATIC_DRAW
  TW_VectorBufferInit(&va, 3, GL_STREAM_DRAW);

  va.data[0] = (TW_Vector){-1, -1, 0};
  va.data[1] = (TW_Vector){0, 1, 0};
  va.data[2] = (TW_Vector){1, -1, 0};

#define WINDOW_DEFAULT (&props)

  TW_Component *root =
      VIEW(.color = {0.3, 0, 0, 0}, .rect = {0, 0, props.width, props.height},
           CHILDREN(
               fps_display = TEXT(.rect = {0, 0, props.width, props.height},
                                  .text = fps_display_string),
               VIEW(.rect = {400, 10, 300, 300}, .color = {1, 0, 1, 0},
                    CHILDREN(
                        TEXT(.rect = {0, 0, 300, 300}, .text = L"test"),
                        VIEW(.rect = {150, 150, 150, 150},
                             .color = {0, 1, 1, 0},
                             CHILDREN(TEXT(.rect = {0, 0, 150, 150},
                                           .text = L"in", ),
                                      VIEW(.rect = {75, 75, 75, 75},
                                           .color = {1, 0, 0, 0},
                                           CHILDREN(TEXT(.rect = {0, 0, 75, 75},
                                                         .text = L"here", )))

                                          ))))));

  /*

    TW_Component *root =
        VIEW(.rect = {0, 0, props.height, props.width},
             CHILDREN(fps_display =
                          TEXT(.rect = {0, 0, props.width, props.height},
                               .text = fps_display_string,
                               .background = {255, 255, 255, 32}),
                      RIGHT(.rect = {0, 0, props.width, props.height},
                            CHILDREN(TEXT(.text = L"Aligned right",
                                          .background = {255, 255, 255, 32}))),
                      CENTER(.rect = {0, 0, props.width, props.height},
                             CHILDREN(TEXT(.text = L"Testing this text thing"),
                                      TEXT(.text = L"META")))));
  */
  while (running) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      switch (ev.type) {
      case SDL_WINDOWEVENT: {
        switch (ev.window.event) {
        case SDL_WINDOWEVENT_SIZE_CHANGED:
          root->rect.w = props.width = ev.window.data1;
          root->rect.h = props.height = ev.window.data2;
          root->resized = 1;
          TW_ComponentRerender(root);
          break;
        }
      } break;
      case SDL_QUIT:
        running = 0;
        break;
      case SDL_MOUSEBUTTONDOWN: {
        TW_Point coord = {ev.motion.x, props.height - ev.motion.y};
        TW_Component *self = component_at_point(root, coord);
        app_log("TW_Component at TW_Point (%f, %f) is %p", coord.x, coord.y,
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
            SDL_SetWindowFullscreen(props.TW_Window, SDL_WINDOW_FULLSCREEN);
          } else {
            app_log("Exit fullscreen mode.");
            SDL_SetWindowFullscreen(props.TW_Window, 0);
          }
        }
      }
    }

    const int start = SDL_GetTicks();
    const int frame_time = (start - previous);
    props.frame_time = frame_time;
    props.passed += frame_time;
    previous = start;

    fps_counter += frame_time;
    if (fps_counter >= 1000) {
      real_fps = fps;
      fps = 1;
      fps_counter -= 1000;
      app_log("FPS: %u", real_fps);
      swprintf(fps_display_string, 0xFF, L"FPS: %u", real_fps);
      TW_ShaderUse(uishader);
      // fps_display->update((TW_Component *)fps_display);
      TW_ComponentRerender(fps_display);
    } else {
      fps++;
    }

    TW_VectorBufferUpdate(&va, 3);

    glViewport(0, 0, props.width, props.height);
    glClearColor(0.2, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    engine_gl_check();

    GLuint program, loc;

    TW_ShaderUse(uishader);
    glGetIntegerv(GL_CURRENT_PROGRAM, &program);
    glUniform1i(glGetUniformLocation(program, "surface"), 0);

    root->render(0, root);

    SDL_GL_SwapWindow(props.TW_Window);

    // FPS limit.
    const int end = SDL_GetTicks();
    if (fps_limit) {
      const int to_delay = ((1000 / fps_limit) - (end - start));
      if (to_delay > 0) {
        SDL_Delay(to_delay);
      }
    }
  }

  return engine_shutdown(props);
}
