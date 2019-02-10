
#include <SDL2/SDL_opengl.h>

#include "log.h"
#include "print.h"

#define TEXTURE_WIDTH (512)
#define TEXTURE_HEIGHT (512)

typedef struct s_font_atlas_glyph_set {
  Uint32 start;
#ifdef USE_GL
  GLuint texture;
#else
  SDL_Texture *texture;
#endif
  SDL_Rect glyphs[256];
} font_atlas_glyph_set;

SDL_Renderer *print_active_renderer = 0;

void print_set_sdl_renderer(SDL_Renderer *renderer) {
  print_active_renderer = renderer;
}

font_atlas_glyph_set *font_atlas_glyph_set_create(font_atlas *atlas, int set) {

  app_debug("Creating glyph set %u", set);

  font_atlas_glyph_set *fs =
      (font_atlas_glyph_set *)malloc(sizeof(font_atlas_glyph_set));

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
  glGenTextures(1, &fs->texture);
  glBindTexture(GL_TEXTURE_2D, fs->texture);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  SDL_LockSurface(glyphs);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0,
               GL_RGBA, GL_UNSIGNED_BYTE, glyphs->pixels);
  glBindTexture(GL_TEXTURE_2D, 0);
  SDL_UnlockSurface(glyphs);
#else
  fs->texture = SDL_CreateTextureFromSurface(renderer, glyphs);
#endif

  SDL_FreeSurface(glyphs);
  return fs;
}

void print_rect(font_atlas *font, SDL_Rect rect, const wchar_t *text) {

#ifdef USE_GL
  GLint previous_viewport[4];
  glGetIntegerv(GL_VIEWPORT, previous_viewport);
  glViewport(rect.x, rect.y, rect.w, rect.h);
#else
  SDL_RenderSetClipRect(print_active_renderer, &rect);
#endif

  SDL_Rect target = rect;
  for (int i = 0; i < wcslen(text); i++) {

    Uint16 c = text[i];
    int set = (c & 0xFF00) >> 8;

    if (font->glyphset[set] == 0) {
      font->glyphset[set] = font_atlas_glyph_set_create(font, set);
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

#ifdef USE_GL
#else
    SDL_RenderCopy(print_active_renderer, s->texture, glyph, &target);
#endif
    target.x += glyph->w;
  }

#ifdef USE_GL
  glViewport(previous_viewport[0], previous_viewport[1], previous_viewport[2],
             previous_viewport[3]);
#else
  SDL_RenderSetClipRect(print_active_renderer, 0);
#endif
}

void print_point(font_atlas *font, SDL_Point point, const wchar_t *text) {

  SDL_Rect target = {point.x, point.y, 0, 0};
  for (int i = 0; i < wcslen(text); i++) {

    Uint16 c = text[i];
    int set = (c & 0xFF00) >> 8;

    if (font->glyphset[set] == 0) {
      font->glyphset[set] = font_atlas_glyph_set_create(font, set);
    }

    const font_atlas_glyph_set *s = font->glyphset[set];
    const SDL_Rect *glyph = &s->glyphs[c & 0xFF];

    target.w = glyph->w;
    target.h = glyph->h;

#ifdef USE_GL

    float glx = glyph->x / (float)TEXTURE_WIDTH;
    float gly = glyph->y / (float)TEXTURE_HEIGHT;
    float grx = glx + glyph->w / (float)TEXTURE_WIDTH;
    float gry = gly + glyph->h / (float)TEXTURE_HEIGHT;

    font->points.data[0].x = target.x;
    font->points.data[0].y = target.y;
    font->points.data[0].z = 0;
    font->uvs.data[0].x = glx;
    font->uvs.data[0].y = gly;

    font->points.data[1].x = target.x + target.w;
    font->points.data[1].y = target.y;
    font->points.data[1].z = 0;
    font->uvs.data[1].x = grx;
    font->uvs.data[1].y = gly;

    font->points.data[2].x = target.x;
    font->points.data[2].y = target.y + target.h;
    font->points.data[2].z = 0;
    font->uvs.data[2].x = glx;
    font->uvs.data[2].y = gry;

    font->points.data[3].x = target.x + target.w;
    font->points.data[3].y = target.y;
    font->points.data[3].z = 0;
    font->uvs.data[3].x = grx;
    font->uvs.data[3].y = gly;

    font->points.data[4].x = target.x + target.w;
    font->points.data[4].y = target.y + target.h;
    font->points.data[4].z = 0;
    font->uvs.data[4].x = grx;
    font->uvs.data[4].y = gry;

    font->points.data[5].x = target.x;
    font->points.data[5].y = target.y + target.h;
    font->points.data[5].z = 0;
    font->uvs.data[5].x = glx;
    font->uvs.data[5].y = gry;

    vector_buffer_update(&font->points, 6);
    point_buffer_update(&font->uvs, 6);

    vector_buffer_bind(&font->points, 0);
    point_buffer_bind(&font->uvs, 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, s->texture);

    glDrawArrays(GL_TRIANGLES, 0, 6);
#else
    SDL_RenderCopy(print_active_renderer, s->texture, glyph, &target);
#endif
    target.x += glyph->w;
  }
}

void print_size(font_atlas *font, const wchar_t *text, SDL_Rect *target) {
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

  vector_buffer_init(&fa->points, 6, GL_STREAM_DRAW);
  point_buffer_init(&fa->uvs, 6, GL_STREAM_DRAW);

  return fa;
}
