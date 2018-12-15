
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "components/center.h"
#include "components/text.h"
#include "core/component.h"
#include "core/log.h"
#include "core/print.h"

#include "organic.h"
organic o = {.depth = 0,
             .sx = 400,
             .sy = 480,
             .dx = 300,
             .dy = 430,
             .age = 0,
             .maturity = 1000,
             .branches = 0};

void screen_render(component *_self) {
  window *props = (window *)_self->window;
  organic_render(props->renderer, &o);
  component_render_children(_self);
}

int immediate(component *_self) {
  window *props = (window *)_self->window;
  organic_update(&o, props->frame_time);
  return 1;
}

// elem_type
int main(int argc, char *args[]) {

  window props;

  int width = 800;
  int height = 600;

  SDL_Init(SDL_INIT_VIDEO);

  props.window = SDL_CreateWindow(
      "Fuuuu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,
      SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

  if (props.window == NULL) {
    app_log("Could not create window...\n");
    return 1;
  }

  props.renderer = SDL_CreateRenderer(props.window, 0, 0);
  props.passed = 0;
  props.frame = 0;

  //  SDL_RenderSetLogicalSize(props.renderer, width, height);
  int real_width, real_height;
  SDL_GetRendererOutputSize(props.renderer, &real_width, &real_height);

  if (real_width != width || real_height != height) {
    app_warning("Real resolution is %u %u, I asked for %u %u. Results might "
                "be... interesting.",
                real_width, real_height, width, height);
    width = real_width;
    height = real_height;
  }

  // SDL_Surface *image = SDL_LoadBMP("block.bmp");
  // props.texture = SDL_CreateTextureFromSurface(props.renderer, image);

  int running = 1;
  int fps_limit = 60, fps = 60, real_fps = 0, fps_counter = 1000;
  int previous = SDL_GetTicks();

  wchar_t fps_display_string[0xFF];
  component *fps_display;

#define WINDOW_DEFAULT (&props)

  component *root = COMPONENT(
      component, .update = &immediate, .render = &screen_render,
      .rect = {0, 0, height, width},
      CHILDREN(
          fps_display = TEXT(.text = fps_display_string),
          CENTER(.rect = {0, 0, width, height},
                 CHILDREN(TEXT(.text = L"Testing this text thing",
                               .rect = {0, 0, 400, 40}),
                          TEXT(.text = L"META", .rect = {0, 0, 100, 30})))));

  int fullscreen = 0;

  TTF_Init();
  default_font = font_atlas_create(props.renderer, "munro.ttf", 32);

  srand(SDL_GetTicks());

  while (running) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_QUIT) {
        running = 0;
      } else if (ev.type == SDL_KEYUP) {
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

    // SDL_Rect r = {0, 0, 320, 240};
    // SDL_SetRenderDrawColor(props.renderer, 255, 0, 255, 0);
    // SDL_RenderFillRect(props.renderer, &r);

    component_update_pass(root);

    root->render(root);
    SDL_RenderPresent(props.renderer);

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
