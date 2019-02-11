
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
#include "core/frame.h"
#include "core/log.h"
#include "core/print.h"

#include "math/math.h"

#include "core/shader.h"

#include "core/texture.h"

#include "core/buffer.h"

// elem_type
int main(int argc, char *args[]) {

  // vector c, d;
  // vector a = {0, 0, 0};
  // vector b = {0, 0, 0};

  // c = vector_cross(a, b);
  // d = vector_add_vector(c, a);

  // int *dd = 0;
  // (*dd) = 2;

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

  default_font = font_atlas_create(RESOURCE("share/dingus/DroidSans.ttf"), 20);

  vector_buffer va;

  frame *frm = frame_create(200, 200);

  // GL_STATIC_DRAW
  vector_buffer_init(&va, 3, GL_STREAM_DRAW);

  va.data[0] = (vector){-1, -1, 0};
  va.data[1] = (vector){0, 1, 0};
  va.data[2] = (vector){1, -1, 0};

  vector_buffer va2;
  point_buffer pa2;

  GLuint texture = load_texture("lord.png");

  // GL_STATIC_DRAW
  vector_buffer_init(&va2, 6, GL_STREAM_DRAW);
  point_buffer_init(&pa2, 6, GL_STREAM_DRAW);

  va2.data[0] = (vector){0, 0, 0};
  va2.data[1] = (vector){0, 500, 0};
  va2.data[2] = (vector){500, 500, 0};
  va2.data[3] = (vector){500, 500, 0};
  va2.data[4] = (vector){500, 0, 0};
  va2.data[5] = (vector){0, 0, 0};

  pa2.data[0] = (point){0, 0};
  pa2.data[1] = (point){0, 1};
  pa2.data[2] = (point){1, 1};
  pa2.data[3] = (point){1, 1};
  pa2.data[4] = (point){1, 0};
  pa2.data[5] = (point){0, 0};

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

    glViewport(0, 0, props.width, props.height);
    glClearColor(0.2, 0, 0, 0);
    engine_gl_check();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    engine_gl_check();

    vector_buffer_update(&va, 3);
    vector_buffer_update(&va2, 6);
    point_buffer_update(&pa2, 6);

    GLuint program;

    /* RENDER TO TEXTURE */
    {
      frame_bind(frm);
      glClearColor(0.4, 0.4, 0, 0);
      engine_gl_check();
      glClear(GL_COLOR_BUFFER_BIT);
      engine_gl_check();

      shader_use(glyphs);

      matrix ortho = matrix_orthogonal_projection(0, 200, 0, 200, 0, 1);
      matrix_gl_uniform("projection", ortho);
      glActiveTexture(GL_TEXTURE0);

      glGetIntegerv(GL_CURRENT_PROGRAM, &program);
      GLuint loc = glGetUniformLocation(program, "glyph_texture");
      glUniform1i(loc, 0);
      /*
          print_point(default_font, (SDL_Point){0, props.height}, L"Visible?");
          print_point(default_font, (SDL_Point){props.width - 200,
         props.height}, L"Visible?");
      */

      // print_rect(default_font, (SDL_Rect){0, 0, 200, 200},
      //            L"Hello World, what is going on in Japan (日本)? I hope
      //            things " L"are going well, because if they were not going
      //            well things " L"would not be going ok, they would be going
      //            rather badly, is " L"not to say that bad is not good, but
      //            inheritly it is.");
      print_point(default_font, (SDL_Point){0, 200}, L"Visible X?");
      frame_unbind(frm);
    }
    // END RENDER TO TEXTURE

    glViewport(0, 0, props.width, props.height);

    shader_use(glyphs);
    glGetIntegerv(GL_CURRENT_PROGRAM, &program);

    matrix ortho =
        matrix_orthogonal_projection(0, props.width, 0, props.height, 0, 1);
    matrix_gl_uniform("projection", ortho);

    vector_buffer_bind(&va2, 0);
    point_buffer_bind(&pa2, 1);

    GLuint loc = glGetUniformLocation(program, "glyph_texture");
    glUniform1i(loc, 0);

    glBindTexture(GL_TEXTURE_2D, frm->texture);
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
    vector_buffer_bind(&va, 0);
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
