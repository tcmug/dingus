
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "component.h"
#include "print.h"

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

typedef struct s_window_props {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;
  int passed;
  int frame;
} window_props;

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

font_atlas *fa;

int text_update(component *this) {
  window_props *props = (window_props *)this->props;
  const wchar_t *text = (const wchar_t *)this->state;
  if (!this->texture) {
    this->texture =
        SDL_CreateTexture(props->renderer, SDL_PIXELFORMAT_RGBA8888,
                          SDL_TEXTUREACCESS_TARGET, this->rect.w, this->rect.h);
  }
  SDL_SetRenderTarget(props->renderer, this->texture);
  SDL_RenderClear(props->renderer);
  print_rect(props->renderer, fa, this->rect, text);
  SDL_SetRenderTarget(props->renderer, 0);
  printf("rendered\n");
}

void texture_render(component *this) {
  window_props *props = (window_props *)this->props;
  if (this->texture) {
    SDL_RenderCopy(props->renderer, this->texture, 0, &this->rect);
  }
}

SDL_Texture *drawText(SDL_Renderer *renderer, TTF_Font *font,
                      const char *text) {

  SDL_Color White = {255, 255, 255};
  SDL_Surface *surface = TTF_RenderText_Solid(font, text, White);

  SDL_Texture *Message = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  return Message;
}

int main(int argc, char *args[]) {

  window_props props;
  int height = 640;
  int width = 480;

  SDL_Init(SDL_INIT_VIDEO);
  props.window = SDL_CreateWindow(
      "Fuuuu",                                        // window title
      SDL_WINDOWPOS_UNDEFINED,                        // initial x position
      SDL_WINDOWPOS_UNDEFINED,                        // initial y position
      height,                                         // width, in pixels
      width,                                          // height, in pixels
      SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI // flags - see below
  );

  // Check that the window was successfully created
  if (props.window == NULL) {
    // In the case that the window could not be made...
    printf("Could not create window...\n");
    return 1;
  }

  props.renderer = SDL_CreateRenderer(props.window, -1, 0);
  props.passed = 0;
  props.frame = 0;

  SDL_Surface *image = SDL_LoadBMP("block.bmp");
  props.texture = SDL_CreateTextureFromSurface(props.renderer, image);

  int running = 1;
  int fps = 60, real_fps = 0, fps_counter = 1000;
  int previous = SDL_GetTicks();
  wchar_t fps_display_string[0xFF];
  component *fps_display =
      NODE(.update = &text_update, .render = &texture_render, .props = &props,
           .state = &fps_display_string, .rect = {0, 0, 200, 100});
  component *root =
      NODE(.update = &immediate, .render = &screen_render, .props = &props,
           .rect = {0, 0, height, width}, .children = LIST(fps_display));

  int fullscreen = 0;

  TTF_Init();
  fa = font_atlas_create(props.renderer, "DroidSans.ttf", 24);

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
          printf("Toggle\n");
          fullscreen = !fullscreen;
          if (fullscreen)
            SDL_SetWindowFullscreen(props.window, SDL_WINDOW_FULLSCREEN);
          else
            SDL_SetWindowFullscreen(props.window, 0);
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
    if (fps_counter >= 1000) {
      real_fps = fps;
      fps = 1;
      fps_counter -= 1000;
      swprintf(fps_display_string, 0xFF, L"FPS: %u", real_fps);
      fps_display->update(fps_display);
    } else {
      fps++;
    }

    root->render(root);

    SDL_RenderPresent(props.renderer);
  }

  SDL_DestroyWindow(props.window);
  SDL_Quit();

  return 0;
}
