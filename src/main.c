
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

  vector a = {0, 0, 0};
  vector b = {0, 1, -1};
  vector c = {1, 0, 0};
  triangle tri = {a, b, c};

  plane p = triangle_to_plane(tri);
  printf("%f %f %f, %f %f %f\n", p.origin.x, p.origin.y, p.origin.z, p.normal.x,
         p.normal.y, p.normal.z);

  vector d = {0, 1, 0};
  printf("%f\n", vector_distance_to_plane(d, p));

  vector dir = {0, -1, -1};
  printf("%i\n", plane_facing_direction(p, dir));

  {
    vector a = {0, 0, 0};
    vector b = {1, 1, 0};
    vector c = {2, 0, 0};
    triangle tri = {a, b, c};
    vector p = {1, 1.1, 0};
    printf("%i\n", point_in_triangle(p, tri));
  }

  printf("\n\n");

  {
    vector a = {0, 0, 0};
    vector b = {10, 10, 0};
    vector c = {20, 0, 0};
    vector p[4] = {{0, 2, 0}, {10, 15, 0}, {10, -2, 0}, {-5, -5, 0}};

    vector closest = closest_point_on_line(a, b, p[0]);
    printf("Closest: %f %f %f\n", closest.x, closest.y, closest.z);

    for (int i = 0; i < 4; i++) {
      closest = closest_point_on_triangle(a, b, c, p[i]);
      printf("Tri closest: %f %f %f\n", closest.x, closest.y, closest.z);
    }
  }
  printf("\n\n");

  {
    sphere s = {{0, 5, 0}, 1};
    plane p = {{0, 0, 0}, vector_normalize((vector){1, 1, 0})};
    vector v = vector_normalize((vector){-1, -1, 0});
    vector closest = sphere_sweep_plane(s, v, p);
    printf("Sweep: %f %f %f\n", closest.x, closest.y, closest.z);
  }
  printf("\n\n");

  {
    sphere s = {{1, 1, 5}, 1};
    triangle t = {{0, 0, 0}, {1, 2, 0}, {2, 0, 0}};
    vector v = vector_normalize((vector){0, 0, -5});
    real d = sphere_sweep_triangle(s, v, t);
    print_vec("Sphere", s.origin);
    printf("Tri Sweep d: %f\n", d);
    // printf("Tri Sweep: %f %f %f\n", closest.x, closest.y, closest.z);
  }
  printf("\n\n");

  {
    sphere s = {{-10, 0, 5}, 1};
    triangle t = {{0, 0, 0}, {1, 2, 0}, {2, 0, 0}};
    vector v = vector_normalize((vector){0, 0, -5});
    real d = sphere_sweep_triangle(s, v, t);
    print_vec("Sphere", s.origin);
    printf("Tri Sweep d: %f\n", d);
    // printf("Tri Sweep: %f %f %f\n", closest.x, closest.y, closest.z);
  }
  printf("\n\n");

  {
    sphere s = {{2, 1, 5}, 1};
    triangle t = {{0, 0, 0}, {1, 2, 0}, {2, 0, 0}};
    vector v = {0, 0, -4};
    real d = sphere_sweep_triangle(s, v, t);
    print_vec("Sphere", s.origin);
    printf("Tri Sweep d: %f\n", d);
    // printf("Tri Sweep: %f %f %f\n", closest.x, closest.y, closest.z);
  }
  printf("\n\n");
  {
    sphere s = {{0, 10, 0}, 1};
    triangle t = {{-5, 5, 5}, {-5, 5, -5}, {5, -5, 0}};
    vector v = {0, -20, 0};
    print_vec("Sphere", s.origin);
    real d = sphere_sweep_triangle(s, v, t);
    vector dest =
        vector_add_vector(s.origin, vector_mul_real(vector_normalize(v), d));
    printf("Tri Sweep d: %f\n", d);
    print_vec("Sphere dest", dest);
    // printf("Tri Sweep: %f %f %f\n", closest.x, closest.y, closest.z);
  }

  // triangle triangles[] = {{{-13, 5, 0}, {-13, 5, 5}, {-10, 0, 0}},
  //                         {{-5, 0, 0}, {5, 0, 5}, {5, 0, 0}},

  //                         {{-10, -10, 0}, {30, -4, 5}, {30, -4, 0}}};
  // int num_spheres = 1;
  // sphere spheres[] = {{{-14, 20, 0}, 3}, {{8, 20, 0}, 4}};
  // vector sphere_vectors[] = {{
  //                                0,
  //                                -0.01,
  //                                0,
  //                            },

  printf("\n\n");
  {
    sphere s = {{-13, 20, 0}, 1};
    triangle t = {{-13, 5, 0}, {-13, 5, 5}, {-10, 0, 0}};
    vector v = {0, -20, 0};
    print_vec("Sphere", s.origin);
    real d = sphere_sweep_triangle(s, v, t);
    vector dest =
        vector_add_vector(s.origin, vector_mul_real(vector_normalize(v), d));
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

  default_font = font_atlas_create(props.renderer,
                                   RESOURCE("share/dingus/DroidSans.ttf"), 32);

  srand(SDL_GetTicks());

  int num_triangles = 3;
  // TRIANGLE ORIENTATION IS IMPORTAAAANT!! Backface lets sphere through in
  // weird ways
  triangle triangles[] = {{{-15, 5, 5}, {-15, 5, -5}, {-10, -5, 0}},
                          {{-5, 0, 0}, {5, 0, 5}, {5, 0, 0}},
                          {{-10, -10, 0}, {30, -4, 5}, {30, -4, 0}}};
  int num_spheres = 2;
  sphere spheres[] = {{{0, 20, 0}, 2}, {{8, 20, 0}, 2.8}};
  vector sphere_vectors[] = {{
                                 0,
                                 -0.01,
                                 0,
                             },
                             {
                                 0,
                                 -0.01,
                                 0,
                             }};
  int hw = props.width / 2;
  int hh = props.height / 2;

  while (running) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      switch (ev.type) {
      case SDL_QUIT:
        running = 0;
        break;
      case SDL_MOUSEBUTTONDOWN: {
        SDL_Point point = {ev.motion.x, ev.motion.y};
        component *self = component_at_point(root, point);
        app_log("component at point (%i, %i) is %p", point.x, point.y, self);
        if (self->click) {
          self->click(self, point);
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
    SDL_SetRenderDrawColor(props.renderer, 0, 0, 0, 0);
    SDL_RenderClear(props.renderer);

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

    component_update_pass(root);

    root->render(root);

    SDL_SetRenderDrawColor(props.renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    for (int i = 0; i < num_triangles; i++) {
      triangle *tri = &triangles[i];
      SDL_RenderDrawLine(props.renderer, hw + (tri->a.x * 10),
                         hh + (tri->a.y * -10), hw + (tri->c.x * 10),
                         hh + (tri->c.y * -10));
    }

    for (int i = 0; i < num_spheres; i++) {
      sphere *s = &spheres[i];

      vector to_move = vector_mul_real(sphere_vectors[i], props.frame_time);
      float closest = 10000;
      for (int j = 0; j < num_triangles; j++) {
        triangle *tri = &triangles[j];
        float d = sphere_sweep_triangle(*s, to_move, *tri);
        if (d >= 0 && d < closest) {
          closest = d;
        }
      }
      // printf("%i => LEN: %f CLOSEST: %f \n", i, vector_length(to_move),
      // closest);

      /*
      Octree based blocks?
      Voxels?

      On contact:
        - collect info on surface
        - point on surface
        - sliding plane
        - distance
        - point on ellipsoid (sphere)
      */

      // Do we have a hit closer than what we intend to move?
      if (closest > vector_length(to_move)) {
        s->origin = vector_add_vector(s->origin, to_move);
      } else {
        s->origin.x = (rand() % 30) - 15;
        s->origin.y = 20;
      }

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
    // SDL_GL_SwapWindow(props.window);
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
