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

// Print text on renderer using font inside rect.
void print_rect(SDL_Renderer *renderer, font_atlas *font, SDL_Rect rect,
                const wchar_t *text);

// Print text on renderer using font.
void print_point(SDL_Renderer *renderer, font_atlas *font, SDL_Point point,
                 const wchar_t *text);

// Get printed text size on renderer using font.
void print_size(SDL_Renderer *renderer, font_atlas *font, const wchar_t *text,
                SDL_Rect *target);

#endif