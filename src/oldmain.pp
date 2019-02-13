
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
#include "math/nettle.h"

void screen_render(component *_self) {
  window *props = (window *)_self->window;
  component_render_children(_self);
}

int immediate(component *_self) {
  window *props = (window *)_self->window;
  return 1;
}

void boo(component *_self, SDL_Point pt) {
  text *self = (text *)_self;
  self->color = (SDL_Color){255, 0, 0, 255};
}

void SDL_Circle(SDL_Renderer *renderer, int cx, int cy, int r) {

  float a = 0;
  int px = cx, py = cy;

  while (a < 3.14159 * 2) {
    int x = cx + (cos(a) * r);
    int y = cy + (sin(a) * r);
    SDL_RenderDrawLine(renderer, px, py, x, y);
    px = x;
    py = y;
    a += 3.14159 / 10;
  }
}

// elem_type
int main(int argc, char *args[]) {

  TW_Vector a = {0, 0, 0};
  TW_Vector b = {0, 1, -1};
  TW_Vector c = {1, 0, 0};
  TW_Triangle tri = {a, b, c};

  TW_Plane p = triangle_to_plane(tri);
  printf("%f %f %f, %f %f %f\n", p.origin.x, p.origin.y, p.origin.z, p.normal.x,
         p.normal.y, p.normal.z);

  TW_Vector d = {0, 1, 0};
  printf("%f\n", TW_VectorDistanceToPlane(d, p));

  TW_Vector dir = {0, -1, -1};
  printf("%i\n", TW_IsPlaneFacingDirection(p, dir));

  {
    TW_Vector a = {0, 0, 0};
    TW_Vector b = {1, 1, 0};
    TW_Vector c = {2, 0, 0};
    TW_Triangle tri = {a, b, c};
    TW_Vector p = {1, 1.1, 0};
    printf("%i\n", TW_PointInTriangle(p, tri));
  }

  printf("\n\n");

  {
    TW_Vector a = {0, 0, 0};
    TW_Vector b = {10, 10, 0};
    TW_Vector c = {20, 0, 0};
    TW_Vector p[4] = {{0, 2, 0}, {10, 15, 0}, {10, -2, 0}, {-5, -5, 0}};

    TW_Vector closest = TW_ClosestPointOnPlane(a, b, p[0]);
    printf("Closest: %f %f %f\n", closest.x, closest.y, closest.z);

    for (int i = 0; i < 4; i++) {
      closest = TW_ClosestPointOnTriangle(a, b, c, p[i]);
      printf("Tri closest: %f %f %f\n", closest.x, closest.y, closest.z);
    }
  }
  printf("\n\n");

  {
    TW_Sphere s = {{0, 5, 0}, 1};
    TW_Plane p = {{0, 0, 0}, TW_VectorNormalize((TW_Vector){1, 1, 0})};
    TW_Vector v = TW_VectorNormalize((TW_Vector){-1, -1, 0});
    TW_Vector closest = TW_SphereSweepPlane(s, v, p);
    printf("Sweep: %f %f %f\n", closest.x, closest.y, closest.z);
  }
  printf("\n\n");

  {
    TW_Sphere s = {{1, 1, 5}, 1};
    TW_Triangle t = {{0, 0, 0}, {1, 2, 0}, {2, 0, 0}};
    TW_Vector v = TW_VectorNormalize((TW_Vector){0, 0, -5});
    TW_Vector TW_Point;
    real d = TW_SphereSweepTriangle(s, v, t, &TW_Point);
    print_vec("Sphere", s.origin);
    printf("Tri Sweep d: %f\n", d);
    // printf("Tri Sweep: %f %f %f\n", closest.x, closest.y, closest.z);
  }
  printf("\n\n");

  {
    TW_Sphere s = {{-10, 0, 5}, 1};
    TW_Triangle t = {{0, 0, 0}, {1, 2, 0}, {2, 0, 0}};
    TW_Vector v = TW_VectorNormalize((TW_Vector){0, 0, -5});
    TW_Vector TW_Point;
    real d = TW_SphereSweepTriangle(s, v, t, &TW_Point);
    print_vec("Sphere", s.origin);
    printf("Tri Sweep d: %f\n", d);
    // printf("Tri Sweep: %f %f %f\n", closest.x, closest.y, closest.z);
  }
  printf("\n\n");

  {
    TW_Sphere s = {{2, 1, 5}, 1};
    TW_Triangle t = {{0, 0, 0}, {1, 2, 0}, {2, 0, 0}};
    TW_Vector v = {0, 0, -4};
    TW_Vector TW_Point;
    real d = TW_SphereSweepTriangle(s, v, t, &TW_Point);
    print_vec("Sphere", s.origin);
    printf("Tri Sweep d: %f\n", d);
    // printf("Tri Sweep: %f %f %f\n", closest.x, closest.y, closest.z);
  }
  printf("\n\n");
  {
    TW_Sphere s = {{0, 10, 0}, 1};
    TW_Triangle t = {{-5, 5, 5}, {-5, 5, -5}, {5, -5, 0}};
    TW_Vector v = {0, -20, 0};
    TW_Vector TW_Point;
    print_vec("Sphere", s.origin);
    real d = TW_SphereSweepTriangle(s, v, t, &TW_Point);
    TW_Vector dest =
        TW_VectorAddVector(s.origin, TW_VectorMulReal(TW_VectorNormalize(v), d));
    printf("Tri Sweep d: %f\n", d);
    print_vec("Sphere dest", dest);
    // printf("Tri Sweep: %f %f %f\n", closest.x, closest.y, closest.z);
  }

  // TW_Triangle triangles[] = {{{-13, 5, 0}, {-13, 5, 5}, {-10, 0, 0}},
  //                         {{-5, 0, 0}, {5, 0, 5}, {5, 0, 0}},

  //                         {{-10, -10, 0}, {30, -4, 5}, {30, -4, 0}}};
  // int num_spheres = 1;
  // TW_Sphere spheres[] = {{{-14, 20, 0}, 3}, {{8, 20, 0}, 4}};
  // TW_Vector sphere_vectors[] = {{
  //                                0,
  //                                -0.01,
  //                                0,
  //                            },

  printf("\n\n");
  {
    TW_Sphere s = {{-13, 20, 0}, 1};
    TW_Triangle t = {{-13, 5, 0}, {-13, 5, 5}, {-10, 0, 0}};
    TW_Vector v = {0, -20, 0};
    TW_Vector TW_Point;
    print_vec("Sphere", s.origin);
    real d = TW_SphereSweepTriangle(s, v, t, &TW_Point);
    TW_Vector dest =
        TW_VectorAddVector(s.origin, TW_VectorMulReal(TW_VectorNormalize(v), d));
    printf("Tri Sweep d: %f\n", d);
    print_vec("Sphere dest", dest);
    // printf("Tri Sweep: %f %f %f\n", closest.x, closest.y, closest.z);
  }

  printf("\n\n");

  window props = engine_init();

  int running = 1;
  int fps_limit = 60, fps = 60, real_fps = 0, fps_counter = 1000;
  int previous = SDL_GetTicks();

  wchar_t fps_display_string[0xFF];
  component *fps_display;

#define WINDOW_DEFAULT (&props)

  component *root = COMPONENT(
      component, .update = &immediate, .render = &screen_render,
      .rect = {0, 0, props.height, props.width},
      CHILDREN(
          fps_display = TEXT(.text = fps_display_string,
                             .background = {255, 255, 255, 32}, .click = boo),
          RIGHT(.rect = {0, 0, props.width, props.height},
                CHILDREN(TEXT(.text = L"Aligned right",
                              .background = {255, 255, 255, 32},
                              .click = boo))),
          CENTER(.rect = {0, 0, props.width, props.height},
                 CHILDREN(TEXT(.text = L"Testing this text thing",
                               .rect = {0, 0, 400, 40}),
                          TEXT(.text = L"META", .rect = {0, 0, 100, 30})))));

  int fullscreen = 0;

  // default_font = font_atlas_create(props.renderer,
  //                                  RESOURCE("share/dingus/DroidSans.ttf"),
  //                                  32);

  srand(SDL_GetTicks());

  int num_triangles = 3;
  // TRIANGLE ORIENTATION IS IMPORTAAAANT!! Backface lets TW_Sphere through in
  // weird ways
  TW_Triangle triangles[] = {{{-15, 5, 5}, {-15, 5, -5}, {-10, -5, 0}},
                          {{-5, 0, 5}, {-5, 0, -5}, {5, 1, 0}},
                          {{-10, -10, 5}, {-10, -10, -5}, {30, -4, 0}}};
  int num_spheres = 2;
  TW_Sphere spheres[] = {{{0, 20, 0}, 2}, {{8, 20, 0}, 2.8}};
  TW_Vector sphere_vectors[] = {{
                                 0,
                                 0,
                                 0,
                             },
                             {
                                 0,
                                 0,
                                 0,
                             }};
  int hw = props.width / 2;
  int hh = props.height / 2;

  shader flat = shader_load(RESOURCE("share/dingus/shaders/flat.vert"), 0,
                            RESOURCE("share/dingus/shaders/flat.frag"));

  vertex_array va;
  vertex_array_init(&va, 3, GL_STREAM_DRAW);

  va.data[0] = (TW_Vector){-1, -1, -5};
  va.data[1] = (TW_Vector){0, 1, -5};
  va.data[2] = (TW_Vector){1, -1, -5};

  while (running) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      switch (ev.type) {
      case SDL_QUIT:
        running = 0;
        break;
      case SDL_MOUSEBUTTONDOWN: {
        SDL_Point TW_Point = {ev.motion.x, ev.motion.y};
        component *self = component_at_point(root, TW_Point);
        app_log("component at TW_Point (%i, %i) is %p", TW_Point.x, TW_Point.y, self);
        if (self->click) {
          self->click(self, TW_Point);
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

    SDL_GetWindowSize(props.window, &root->rect.w, &root->rect.h);

    fps_counter += frame_time;
    if (fps_counter >= 1000) {
      real_fps = fps;
      fps = 1;
      fps_counter -= 1000;
      swprintf(fps_display_string, 0xFF, L"FPS: %u", real_fps);
      fps_display->update((component *)fps_display);
    } else {
      fps++;
    }

    // SDL_SetRenderDrawColor(props.renderer, 0, 0, 0, 0);
    // SDL_RenderClear(props.renderer);
    // component_update_pass(root);
    // root->render(root);
    // SDL_SetRenderDrawColor(props.renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    /*
    for (int i = 0; i < num_triangles; i++) {
      TW_Triangle *tri = &triangles[i];
      SDL_RenderDrawLine(props.renderer, hw + (tri->a.x * 10),
                         hh + (tri->a.y * -10), hw + (tri->c.x * 10),
                         hh + (tri->c.y * -10));
    }


    for (int i = 0; i < num_spheres; i++) {
      TW_Sphere *s = &spheres[i];

      TW_Vector gravity = {0, -10 * (props.frame_time / 1000.0), 0};
      sphere_vectors[i] = TW_VectorAddVector(sphere_vectors[i], gravity);

      TW_Collision col;
      col.object = *s;
      col.velocity =
          TW_VectorMulReal(sphere_vectors[i], props.frame_time / 1000.0);

      while (TW_VectorLength(col.velocity) > 0.0001) {
        if (TW_TestTriangles(&col, num_triangles, triangles)) {

          real movement_distance = TW_VectorLength(col.velocity);
          TW_Vector movement_normal = TW_VectorNormalize(col.velocity);

          TW_Vector to_move =
              TW_VectorMulReal(movement_normal, col.distance - 0.0001);
          real movement_left = movement_distance - col.distance;

          s->origin = TW_VectorAddVector(s->origin, to_move);
          col.velocity = TW_VectorMulReal(movement_normal, movement_left);

          // Reflect based on hit normal:
          TW_Vector normal =
              TW_VectorNormalize(TW_VectorSubVector(s->origin, col.TW_Point));

          col.velocity = TW_VectorReflect(col.velocity, normal);
          sphere_vectors[i] = TW_VectorReflect(sphere_vectors[i], normal);

        } else {
          s->origin = TW_VectorAddVector(s->origin, col.velocity);
          col.velocity = (TW_Vector){0, 0, 0};
        }
      }

      if (s->origin.y < -10) {
        s->origin.y = 10;
        s->origin.x = (rand() % 10) - 5;
        sphere_vectors[i] = (TW_Vector){0, 0, 0};
      }

      //Octree based blocks?
      //Voxels?

      // On contact:
      // - collect info on surface
      // - TW_Point on surface
      // - sliding TW_Plane
      // - distance
      // - TW_Point on ellipsoid (TW_Sphere)


      SDL_Circle(props.renderer, hw + (s->origin.x * 10),
                 hh + (s->origin.y * -10), s->radius * 10);
    }
    // SDL_Rect r = {0, 0, 320, 240};
    // SDL_SetRenderDrawBlendMode(props.renderer, SDL_BLENDMODE_BLEND);
    // SDL_SetRenderDrawColor(props.renderer, 255, 0, 255, 128);
    // SDL_RenderCopy(props.renderer, image, 0, &r);
    // SDL_RenderFillRect(props.renderer, &r);

    print_point(props.renderer, default_font, (SDL_Point){0, props.height / 2},
                L"Lefty");

    SDL_RenderPresent(props.renderer);
    */

    TW_Matrix projection = TW_MatrixPerspectiveProjection(0.01, 1000, 70, 1);
    TW_Matrix view = TW_MatrixIdentity();
    TW_Matrix model = TW_MatrixIdentity();

    glClearColor(0.2, 0, 0, 0);
    engine_gl_check();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    engine_gl_check();

    vertex_buffer_update(&va, 3);
    shader_use(flat);

    TW_MatrixGLUniform("projection", projection);
    TW_MatrixGLUniform("view", view);
    TW_MatrixGLUniform("model", model);

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
