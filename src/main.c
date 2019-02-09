
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
#include "core/component.h"
#include "core/engine.h"
#include "core/log.h"
#include "core/print.h"

#include "core/shader.h"
#include "core/vertex_array.h"
#include "math/math.h"

// elem_type
int main(int argc, char *args[]) {

  window props = engine_init();

  int running = 1;
  int fps_limit = 60, fps = 60, real_fps = 0, fps_counter = 1000;
  int previous = SDL_GetTicks();

  wchar_t fps_display_string[0xFF];
  component *fps_display;

  int fullscreen = 0;

  srand(SDL_GetTicks());

  shader flat = shader_load(RESOURCE("share/dingus/shaders/flat.vert"), 0,
                            RESOURCE("share/dingus/shaders/flat.frag"));

  shader glyphs = shader_load(RESOURCE("share/dingus/shaders/glyph.vert"), 0,
                              RESOURCE("share/dingus/shaders/glyph.frag"));

  vertex_array va;

  // GL_STATIC_DRAW
  vertex_array_init(&va, 3, GL_STREAM_DRAW);

  va.data[0] = (vector){-1, -1, 0};
  va.data[1] = (vector){0, 1, 0};
  va.data[2] = (vector){1, -1, 0};

  vertex_array va2;

  // GL_STATIC_DRAW
  vertex_array_init(&va2, 6, GL_STREAM_DRAW);

  va2.data[0] = (vector){0, 0, 0};
  va2.data[1] = (vector){50, 0, 0};
  va2.data[2] = (vector){0, 50, 0};
  va2.data[3] = (vector){50, 0, 0};
  va2.data[4] = (vector){50, 50, 0};
  va2.data[5] = (vector){0, 50, 0};

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  while (running) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      switch (ev.type) {
      case SDL_QUIT:
        running = 0;
        break;
      case SDL_MOUSEBUTTONDOWN: {
        SDL_Point point = {ev.motion.x, ev.motion.y};
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
            SDL_SetWindowFullscreen(props.window, SDL_WINDOW_FULLSCREEN);
          } else {
            app_log("Exit fullscreen mode.");
            SDL_SetWindowFullscreen(props.window, 0);
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
    } else {
      fps++;
    }

    glClearColor(0.2, 0, 0, 0);
    engine_gl_check();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    engine_gl_check();

    vertex_buffer_update(&va, 3);
    vertex_buffer_update(&va2, 6);

    matrix ortho =
        matrix_orthogonal_projection(0, props.width, props.height, 0, 0, 1);

    shader_use(glyphs);

    matrix_gl_uniform("projection", ortho);
    matrix_gl_uniform("view", matrix_identity());
    matrix_gl_uniform("model", matrix_identity());

    vertex_buffer_bind(&va2, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    engine_gl_check();
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    engine_gl_check();

    vector dir = (vector){1, 1, -5};
    matrix projection = matrix_perspective_projection(0.01, 1000, 70, 1);
    matrix view = matrix_from_vector((vector){0, 0, 5}, (vector){0, 0, 0},
                                     (vector){0, 1, 0});
    matrix model = matrix_rotation(0, props.passed * 0.001, 0);

    shader_use(flat);

    matrix_gl_uniform("projection", projection);
    matrix_gl_uniform("view", view);
    matrix_gl_uniform("model", model);

    // GLuint program;
    // glGetIntegerv(GL_CURRENT_PROGRAM, &program);
    // app_log("loc: %u %u", glGetAttribLocation(program, "vertex"),
    //         glGetUniformLocation(program, "view"));
    vertex_buffer_bind(&va, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    engine_gl_check();
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    engine_gl_check();

    SDL_GL_SwapWindow(props.window);
    // FPS limit.
    const int end = SDL_GetTicks();
    if (fps_limit) {
      const int to_delay = ((1000 / fps_limit) - (end - start));
      if (to_delay > 0) {
        SDL_Delay(to_delay);
      }
    }
  }

  SDL_DestroyWindow(props.window);
  SDL_Quit();

  return 0;
}
