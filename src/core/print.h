#ifndef PRINT_H
#define PRINT_H

// Comment below line to use default SDL implementation
#define USE_GL

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#ifdef USE_GL
#include "buffer.h"
#endif

typedef struct font_t_atlas {
  TTF_Font *font;
  struct font_t_atlas_glyph_set *glyphset[256];
  struct font_t_atlas_glyph_set *active_glyphset;
#ifdef USE_GL
  int to_render;
  TW_VectorBuffer points;
  TW_PointBuffer uvs;
#endif
} font_atlas;

void print_set_sdl_renderer(SDL_Renderer *renderer);

font_atlas *font_atlas_create(const char *fontName, int size);

// Print text on renderer using font inside rect.
void print_rect(font_atlas *font, SDL_Rect rect, const wchar_t *text);

// Print text on renderer using font.
void print_point(font_atlas *font, SDL_Point TW_Point, const wchar_t *text);

// Get printed text size on renderer using font.
void print_size(font_atlas *font, const wchar_t *text, TW_Rectangle *target);

#endif