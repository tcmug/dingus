
#include <SDL2/SDL_opengl.h>

#include "log.h"
#include "print.h"

#define TEXTURE_WIDTH (512)
#define TEXTURE_HEIGHT (512)

typedef struct font_t_atlas_glyph_set {
  Uint32 start;
#ifdef USE_GL
  GLuint TW_Texture;
#else
  SDL_Texture *TW_Texture;
#endif
  SDL_Rect glyphs[256];
} font_atlas_glyph_set;

SDL_Renderer *print_active_renderer = 0;

void print_set_sdl_renderer(SDL_Renderer *renderer) {
  print_active_renderer = renderer;
}

font_atlas_glyph_set *font_atlas_glyph_set_create(font_atlas *atlas, int set) {

  app_log("Creating glyph set %u", set);

  font_atlas_glyph_set *fs =
      (font_atlas_glyph_set *)malloc(sizeof(font_atlas_glyph_set));
  // memset(fs, 0, sizeof(font_atlas_glyph_set));

  SDL_Color white = {255, 255, 255, 0};

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

  SDL_SaveBMP(glyphs, "glyph.bmp");

#ifdef USE_GL
  SDL_LockSurface(glyphs);
  glGenTextures(1, &fs->TW_Texture);
  glBindTexture(GL_TEXTURE_2D, fs->TW_Texture);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0,
               GL_RGBA, GL_UNSIGNED_BYTE, glyphs->pixels);
  glBindTexture(GL_TEXTURE_2D, 0);
  SDL_UnlockSurface(glyphs);

#else
  fs->TW_Texture = SDL_CreateTextureFromSurface(renderer, glyphs);
#endif

  SDL_FreeSurface(glyphs);

  return fs;
}

void _print_flush(font_atlas *font) {
  if (font->to_render == 0) {
    return;
  }
  TW_VectorBufferUpdate(&font->points, font->to_render);
  TW_PointBufferUpdate(&font->uvs, font->to_render);

  TW_VectorBufferBind(&font->points, 0);
  TW_PointBufferBind(&font->uvs, 1);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, font->active_glyphset->TW_Texture);

  glDrawArrays(GL_TRIANGLES, 0, font->to_render);
  font->to_render = 0;
}

void _print_push_glyph(font_atlas *font, SDL_Rect glyph, SDL_Rect target) {

#ifdef USE_GL

  float glx = glyph.x / (float)TEXTURE_WIDTH;
  float gry = glyph.y / (float)TEXTURE_HEIGHT;
  float grx = glx + glyph.w / (float)TEXTURE_WIDTH;
  float gly = gry + glyph.h / (float)TEXTURE_HEIGHT;

  font->points.data[font->to_render].x = target.x;
  font->points.data[font->to_render].y = target.y - target.h;
  font->points.data[font->to_render].z = 0;
  font->uvs.data[font->to_render].x = glx;
  font->uvs.data[font->to_render].y = gly;
  font->to_render++;

  font->points.data[font->to_render].x = target.x;
  font->points.data[font->to_render].y = target.y;
  font->points.data[font->to_render].z = 0;
  font->uvs.data[font->to_render].x = glx;
  font->uvs.data[font->to_render].y = gry;
  font->to_render++;

  font->points.data[font->to_render].x = target.x + target.w;
  font->points.data[font->to_render].y = target.y - target.h;
  font->points.data[font->to_render].z = 0;
  font->uvs.data[font->to_render].x = grx;
  font->uvs.data[font->to_render].y = gly;
  font->to_render++;

  font->points.data[font->to_render].x = target.x + target.w;
  font->points.data[font->to_render].y = target.y - target.h;
  font->points.data[font->to_render].z = 0;
  font->uvs.data[font->to_render].x = grx;
  font->uvs.data[font->to_render].y = gly;
  font->to_render++;

  font->points.data[font->to_render].x = target.x;
  font->points.data[font->to_render].y = target.y;
  font->points.data[font->to_render].z = 0;
  font->uvs.data[font->to_render].x = glx;
  font->uvs.data[font->to_render].y = gry;
  font->to_render++;

  font->points.data[font->to_render].x = target.x + target.w;
  font->points.data[font->to_render].y = target.y;
  font->points.data[font->to_render].z = 0;
  font->uvs.data[font->to_render].x = grx;
  font->uvs.data[font->to_render].y = gry;
  font->to_render++;

  if (font->to_render >= font->points.size) {
    _print_flush(font);
  }

#else
  SDL_RenderCopy(print_active_renderer, s->TW_Texture, glyph, &target);
#endif
}

void print_rect(font_atlas *font, SDL_Rect rect, const wchar_t *text) {

  SDL_Rect target = {rect.x, rect.y, 0, 0};
  for (int i = 0; i < wcslen(text); i++) {

    Uint16 c = text[i];
    int set = (c & 0xFF00) >> 8;

    if (font->glyphset[set] == 0) {
      font->glyphset[set] = font_atlas_glyph_set_create(font, set);
    }

    const font_atlas_glyph_set *s = font->glyphset[set];
    const SDL_Rect *glyph = &s->glyphs[c & 0xFF];

    // Track glyph TW_Texture changes & render any glyphs that are on the
    // previous TW_Texture.
    if (font->active_glyphset != s) {
#ifdef USE_GL
      _print_flush(font);
#endif
      font->active_glyphset = font->glyphset[set];
    }

    target.w = glyph->w;
    target.h = glyph->h;

    if (target.x + glyph->w > rect.x + rect.w) {
      target.y -= glyph->h;
      target.x = rect.x;
    }

    _print_push_glyph(font, *glyph, target);

    target.x += glyph->w;
  }

#ifdef USE_GL
  _print_flush(font);
#endif
}

void print_point(font_atlas *font, SDL_Point TW_Point, const wchar_t *text) {

  SDL_Rect target = {TW_Point.x, TW_Point.y, 0, 0};
  for (int i = 0; i < wcslen(text); i++) {

    Uint16 c = text[i];
    int set = (c & 0xFF00) >> 8;

    if (font->glyphset[set] == 0) {
      font->glyphset[set] = font_atlas_glyph_set_create(font, set);
    }

    const font_atlas_glyph_set *s = font->glyphset[set];
    const SDL_Rect *glyph = &s->glyphs[c & 0xFF];

    // Track glyph TW_Texture changes & render any glyphs that are on the
    // previous TW_Texture.
    if (font->active_glyphset != s) {
#ifdef USE_GL
      _print_flush(font);
#endif
      font->active_glyphset = font->glyphset[set];
    }

    target.w = glyph->w;
    target.h = glyph->h;

    _print_push_glyph(font, *glyph, target);

    target.x += glyph->w;
  }

#ifdef USE_GL
  _print_flush(font);
#endif
}

void print_size(font_atlas *font, const wchar_t *text, TW_Rectangle *target) {
  target->w = 0;
  target->h = 0;

  for (int i = 0; i < wcslen(text); i++) {

    Uint16 c = text[i];
    int set = (c & 0xFF00) >> 8;

    if (font->glyphset[set] == 0) {
      font->glyphset[set] = font_atlas_glyph_set_create(font, set);
    }

    const font_atlas_glyph_set *s = font->glyphset[set];
    const SDL_Rect *glyph = &s->glyphs[c & 0xFF];

    target->w += glyph->w;
    target->h = glyph->h > target->h ? glyph->h : target->h;
  }
}

font_atlas *font_atlas_create(const char *fontName, int size) {

  app_log("Creating font atlas %s", fontName);

  font_atlas *fa = (font_atlas *)malloc(sizeof(font_atlas));
  memset(fa, 0, sizeof(font_atlas));

  fa->font = TTF_OpenFont(fontName, size);
  if (!fa->font) {
    app_log("Unable to load %s", fontName);
  }

#ifdef USE_GL
  fa->to_render = 0;
  TW_VectorBufferInit(&fa->points, 6 * 10, GL_STREAM_DRAW);
  TW_PointBufferInit(&fa->uvs, 6 * 10, GL_STREAM_DRAW);
#endif
  return fa;
}
