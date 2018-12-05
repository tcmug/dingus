
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "components/text.h"
#include "core/component.h"
#include "core/log.h"
#include "core/print.h"

void screen_render(component *self) { component_render_children(self); }
int immediate(component *self) { return 1; }

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
  component *fps_display1, *fps_display2;

  component *root = COMPONENT_ALLOC(
      component, .update = &immediate, .render = &screen_render,
      .window = &props, .rect = {0, 0, height, width},
      .children =
          LIST(fps_display1 =
                   (component *)TEXT(.window = &props,
                                     .state = {.text = fps_display_string},
                                     .rect = {0, 0, 200, 100}),
               fps_display2 =
                   (component *)TEXT(.window = &props,
                                     .state = {.text = fps_display_string},
                                     .rect = {width / 2, 0, width, 100})));

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
    props.passed += frame_time;
    previous = start;

    SDL_GetWindowSize(props.window, &root->rect.w, &root->rect.h);
    SDL_SetRenderDrawColor(props.renderer, 0, 0, 0, 0);
    SDL_RenderClear(props.renderer);

    fps_counter += frame_time;
    if (fps_counter % 100) {
      component_resize(fps_display2, 400, 400);
      component_move(fps_display1, fps_display1->rect.x,
                     (fps_display1->rect.y + 1) % 100);
    }

    if (fps_counter >= 1000) {
      real_fps = fps;
      fps = 1;
      fps_counter -= 1000;
      swprintf(fps_display_string, 0xFF, L"FPS: %u", real_fps);
      fps_display1->update((component *)fps_display1);
      fps_display2->update((component *)fps_display2);
    } else {
      fps++;
    }

    SDL_Rect r = {0, 0, 320, 240};
    SDL_SetRenderDrawColor(props.renderer, 255, 0, 255, 0);
    SDL_RenderFillRect(props.renderer, &r);

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
