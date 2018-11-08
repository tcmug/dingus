#ifndef PRINT_H
#define PRINT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct s_font_atlas {
  TTF_Font *font;
  struct s_font_atlas_glyph_set *glyphset[256];
} font_atlas;

font_atlas *font_atlas_create(SDL_Renderer *renderer, const char *fontName,
                              int size);

void print_rect(SDL_Renderer *renderer, font_atlas *font, SDL_Rect rect,
                const wchar_t *text);

#endif