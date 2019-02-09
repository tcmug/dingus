#ifndef PRINT_H
#define PRINT_H

// Comment below line to use default SDL implementation
#define USE_GL (1)

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct s_font_atlas {
  TTF_Font *font;
  struct s_font_atlas_glyph_set *glyphset[256];
} font_atlas;

void print_set_sdl_renderer(SDL_Renderer *renderer);

font_atlas *font_atlas_create(const char *fontName, int size);

// Print text on renderer using font inside rect.
void print_rect(font_atlas *font, SDL_Rect rect, const wchar_t *text);

// Print text on renderer using font.
void print_point(font_atlas *font, SDL_Point point, const wchar_t *text);

// Get printed text size on renderer using font.
void print_size(font_atlas *font, const wchar_t *text, SDL_Rect *target);

#endif