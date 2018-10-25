
#include "print.h"

#define TEXTURE_WIDTH (256)
#define TEXTURE_HEIGHT (256)

typedef struct s_font_atlas_glyph_set {
  Uint32 start;
  SDL_Texture *texture;
  SDL_Rect glyphs[256];
} font_atlas_glyph_set;

font_atlas_glyph_set *font_atlas_glyph_set_create(SDL_Renderer *renderer,
                                                  font_atlas *atlas, int set) {

  font_atlas_glyph_set *fs =
      (font_atlas_glyph_set *)malloc(sizeof(font_atlas_glyph_set));

  SDL_Color white = {255, 255, 255};

  Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#endif

  SDL_Surface *glyphs = SDL_CreateRGBSurface(0, TEXTURE_WIDTH, TEXTURE_HEIGHT,
                                             32, rmask, gmask, bmask, amask);

  SDL_Rect destRect = {0, 0, 0, 0};

  int start = 0;
  if (set == 0) {
    start = 1;
  }

  for (int c = start; c < 256; c++) {
    Uint16 glyph = (set << 8) | c;

    SDL_Surface *surface = TTF_RenderGlyph_Blended(atlas->font, glyph, white);
    if (!surface) {
      continue;
    }

    destRect.x += destRect.w;
    destRect.w = surface->w;
    destRect.h = surface->h;

    if (destRect.x + surface->w > TEXTURE_WIDTH) {
      destRect.x = 0;
      destRect.y += surface->h;
    }

    fs->glyphs[c] = destRect;

    SDL_BlitSurface(surface, 0, glyphs, &destRect);
    SDL_FreeSurface(surface);
  }

  fs->texture = SDL_CreateTextureFromSurface(renderer, glyphs);
  SDL_FreeSurface(glyphs);
  return fs;
}

void print_rect(SDL_Renderer *renderer, font_atlas *font, SDL_Rect rect,
                const wchar_t *text) {

  SDL_RenderSetClipRect(renderer, &rect);
  SDL_Rect target = rect;

  for (int i = 0; i < wcslen(text); i++) {

    Uint16 c = text[i];
    int set = (c & 0xFF00) >> 8;

    if (font->glyphset[set] == 0) {
      font->glyphset[set] = font_atlas_glyph_set_create(renderer, font, set);
    }

    const font_atlas_glyph_set *s = font->glyphset[set];
    const SDL_Rect *glyph = &s->glyphs[c & 0xFF];

    target.w = glyph->w;
    target.h = glyph->h;

    if (target.x + target.w > rect.x + rect.w) {
      target.x = rect.x;
      target.y = target.y + glyph->h;
      if (target.y > rect.y + rect.h)
        break;
    }

    SDL_SetTextureColorMod(s->texture, 255, 0, 0);
    SDL_RenderCopy(renderer, s->texture, glyph, &target);

    target.x += glyph->w;
  }

  SDL_RenderSetClipRect(renderer, 0);
}

font_atlas *font_atlas_create(SDL_Renderer *renderer, const char *fontName,
                              int size) {

  font_atlas *fa = (font_atlas *)malloc(sizeof(font_atlas));
  memset(fa, 0, sizeof(font_atlas));

  fa->font = TTF_OpenFont(fontName, size);
  if (!fa->font) {
    printf("Unable to load %s\n", fontName);
  }

  return fa;
}
