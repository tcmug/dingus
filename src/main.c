
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

#define PI 3.14159

float linear(float t, float b, float c, float d) { return c * t / d + b; }
float elastic(float t, float b, float c, float d) {
  if (t == 0)
    return b;
  if ((t /= d) == 1)
    return b + c;
  float p = d * .3f;
  float a = c;
  float s = p / 4;
  float postFix =
      a *
      pow(2,
          10 * (t -= 1)); // this is a fix, again, with post-increment operators
  return -(postFix * sin((t * d - s) * (2 * PI) / p)) + b;
}

float easeOut(float t, float b, float c, float d) {
  t /= d;
  return -c * t * (t - 2) + b;
};

int map[12][25] = {
    {1, 1, 2, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    {1, 0, 0, 0, 2, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1},
    {1, 1, 2, 1, 2, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0},
    {1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1},
    {0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0},
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 2, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
};

void screen_render(component *this) {
  /*
window_props *props = (window_props *)this->props;

int scale = 1;
int offsetx =
((this->rect.w / scale) / 2) - 16 + sin(props->passed / 5000.0) * 600;
int offsety =
((this->rect.h / scale) / 2) - 16 + sin(props->passed / 5000.0) * 300;

int blockw = 32;
int blockh = 32;
int bw = blockw;
// easeOut(props->passed, 0, blockw, frametime / 2);
int bh = blockh;
// easeOut(props->passed, 0, blockh, frametime / 2);
int cell_offset_x = blockw / 2;
int cell_offset_y = blockh / 4;

for (int frame = 0; frame < 8; frame++) {
for (int y = 0; y < 5; y++) {
for (int x = 0; x < 5; x++) {
  int cell = map[frame][x + (y * 5)];
  if (cell > 0) {
    int px = offsetx + (cell_offset_x * x - y * cell_offset_x) +
             (frame * 7 * cell_offset_x);
    int py = (offsety + ((x + y) * cell_offset_y) - blockh) +
             (frame * 7 * cell_offset_y);
    SDL_Rect dstrect = {px * scale, py * scale, bw * scale, bh * scale};
    SDL_Rect source = {(cell - 1) * blockw, 0, blockw, blockh};
    SDL_RenderCopy(props->renderer, props->texture, &source, &dstrect);
  }
}
}
}
*/
  node_render_children(this);
}

int immediate(component *this) { return 1; }

int main(int argc, char *args[]) {

  window props;
  int width = 640;
  int height = 480;

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
    app_warning("Real resolution is %u %u, I asked for %u %u. Results might be... interesting.", real_width, real_height, width, height);
    width = real_width;
    height = real_height;
  }

  // SDL_Surface *image = SDL_LoadBMP("block.bmp");
  // props.texture = SDL_CreateTextureFromSurface(props.renderer, image);

  int running = 1;
  int fps = 60, real_fps = 0, fps_counter = 1000;
  int previous = SDL_GetTicks();

  wchar_t fps_display_string[0xFF];
  component *fps_display1, *fps_display2;

  component *root =
      NODE(.update = &immediate, .render = &screen_render, .window = &props,
           .rect = {0, 0, height, width},
           .children =
               LIST(fps_display1 =
                        TEXT(.window = &props, .state = &fps_display_string,
                             .rect = {0, 0, 200, 100}),
                    fps_display2 =
                        TEXT(.window = &props, .state = &fps_display_string,
                             .rect = {width/2, 0, width, 100})));

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

    SDL_GetWindowSize(props.window, &root->rect.w, &root->rect.h);
    SDL_SetRenderDrawColor(props.renderer, 0, 0, 0, 0);
    SDL_RenderClear(props.renderer);

    int now = SDL_GetTicks();
    int frame_time = (now - previous);
    props.passed += frame_time;
    previous = now;
    fps_counter += frame_time;
    if (fps_counter % 100) {
      node_resize(fps_display2, 400, 400);
      node_move(fps_display1, fps_display1->rect.x, (fps_display1->rect.y + 1) % 100);
    }
    if (fps_counter >= 1000) {
      real_fps = fps;
      fps = 1;
      fps_counter -= 1000;
      swprintf(fps_display_string, 0xFF, L"FPS: %u", real_fps);
      fps_display1->update(fps_display1);
      fps_display2->update(fps_display2);
    } else {
      fps++;
    }

    SDL_Rect r = {0, 0, 320, 240};
    SDL_SetRenderDrawColor(props.renderer, 255, 0, 255, 0);
    SDL_RenderFillRect(props.renderer, &r);


    root->render(root);
    SDL_RenderPresent(props.renderer);
  }

  SDL_DestroyWindow(props.window);
  SDL_Quit();

  return 0;
}
