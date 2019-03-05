#ifndef PRINT_H
#define PRINT_H

#include "buffer.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct TW_FontAtlas_t {
  TTF_Font *font;
  struct TW_Glyph_t *glyphs;
  struct TW_Glyph_t *active_glyph;

  int to_render;
  TW_Vector3Buffer points;
  TW_Vector2Buffer uvs;
} TW_FontAtlas;

void print_set_sdl_renderer(SDL_Renderer *renderer);

TW_FontAtlas *TW_FontLoad(const char *fontName, int size);

// Print text on renderer using font inside rect.
void print_rect(TW_FontAtlas *font, TW_Rectangle rect, const char *text);

// Print text on renderer using font.
void print_point(TW_FontAtlas *font, SDL_Point TW_Vector2, const char *text);

// Get printed text size on renderer using font.
void print_size(TW_FontAtlas *font, const char *text, TW_Rectangle *target);

#endif