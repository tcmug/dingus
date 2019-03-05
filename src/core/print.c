
#include <SDL2/SDL_opengl.h>

#include "log.h"
#include "print.h"
#include "print_utf8.h"

#define TEXTURE_WIDTH (512)
#define TEXTURE_HEIGHT (512)

typedef struct TW_Glyph_t {
  Uint16 code;
  GLuint texture;
  int w, h;
  int minx, maxx, miny, maxy;
  int advance;
  struct TW_Glyph_t *next;
  struct TW_Glyph_t *prev;
} TW_Glyph;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
const Uint32 rmask = 0xff000000;
const Uint32 gmask = 0x00ff0000;
const Uint32 bmask = 0x0000ff00;
const Uint32 amask = 0x000000ff;
#else
const Uint32 rmask = 0x000000ff;
const Uint32 gmask = 0x0000ff00;
const Uint32 bmask = 0x00ff0000;
const Uint32 amask = 0xff000000;
#endif

TW_Glyph *TW_FontCreateGlyph(TW_FontAtlas *atlas, Uint16 code) {

  TW_Glyph *g = (TW_Glyph *)malloc(sizeof(TW_Glyph));
  g->code = code;

  SDL_Color white = {255, 255, 255, 0};
  SDL_Surface *ttf_glyph = TTF_RenderGlyph_Blended(atlas->font, code, white);

  const int width = ttf_glyph->w;
  const int height = ttf_glyph->h;

  TTF_GlyphMetrics(atlas->font, code, &g->minx, &g->maxx, &g->miny, &g->maxy,
                   &g->advance);
  // app_log("%i %i %i %i %i /%i %i\n", g->minx, g->maxx, g->miny, g->maxy,
  //         g->advance, width, height);

  SDL_Surface *glyph =
      SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);
  SDL_BlitSurface(ttf_glyph, 0, glyph, 0);
  SDL_FreeSurface(ttf_glyph);

  g->w = width;
  g->h = height;

  SDL_LockSurface(glyph);
  glGenTextures(1, &g->texture);
  glBindTexture(GL_TEXTURE_2D, g->texture);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, glyph->pixels);
  glBindTexture(GL_TEXTURE_2D, 0);
  SDL_UnlockSurface(glyph);
  SDL_FreeSurface(glyph);

  return g;
}

void _print_flush(TW_FontAtlas *font) {

  if (font->to_render == 0) {
    return;
  }

  TW_Vector3BufferUpdate(&font->points, font->to_render);
  TW_Vector2BufferUpdate(&font->uvs, font->to_render);

  TW_Vector3BufferBind(&font->points, 0);
  TW_Vector2BufferBind(&font->uvs, 1);

  glBindTexture(GL_TEXTURE_2D, font->active_glyph->texture);

  glDrawArrays(GL_TRIANGLES, 0, font->to_render);
  font->to_render = 0;
}

void _print_push_glyph(TW_FontAtlas *font, TW_Rectangle target) {

  float glx = 0;
  float gry = 0;
  float grx = 1;
  float gly = 1;

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
}

void print_rect(TW_FontAtlas *font, TW_Rectangle rect, const char *text) {

  GLint program;
  glActiveTexture(GL_TEXTURE0);
  glGetIntegerv(GL_CURRENT_PROGRAM, &program);
  GLint loc = glGetUniformLocation(program, "surface");
  glUniform1i(loc, 0);

  TW_Rectangle target = {rect.x, rect.y, 0, 0};
  uint32_t c;

  font->active_glyph = 0;

  const char *i = text;
  while (*i) {

    c = to_cp(i);
    i += codepoint_len(c);

    TW_Glyph *glyph = 0;
    DL_FOREACH(font->glyphs, glyph) {
      if (glyph->code == c) {
        break;
      }
    }

    if (!glyph) {
      glyph = TW_FontCreateGlyph(font, c);
      DL_APPEND(font->glyphs, glyph);
    }

    if (font->active_glyph != glyph) {
      _print_flush(font);
      font->active_glyph = glyph;
    }

    target.w = glyph->w;
    target.h = glyph->h;

    if (target.x + glyph->w > rect.x + rect.w) {
      target.y -= glyph->h;
      target.x = rect.x;
    }

    _print_push_glyph(font, target);

    target.x += glyph->advance;
  }

  _print_flush(font);
}

TW_FontAtlas *TW_FontLoad(const char *fontName, int size) {

  app_log("Creating font atlas %s", fontName);

  TW_FontAtlas *fa = (TW_FontAtlas *)malloc(sizeof(TW_FontAtlas));
  memset(fa, 0, sizeof(TW_FontAtlas));

  fa->font = TTF_OpenFont(fontName, size);
  if (!fa->font) {
    app_log("Unable to load %s", fontName);
  }

  fa->glyphs = 0;
  fa->to_render = 0;
  fa->active_glyph = 0;

  TW_Vector3BufferInit(&fa->points, 6 * 10, GL_STREAM_DRAW);
  TW_Vector2BufferInit(&fa->uvs, 6 * 10, GL_STREAM_DRAW);

  // Prepopulate regular asciis.
  if (!fa->glyphs) {
    for (Uint16 i = 1; i < 256; i++) {
      TW_Glyph *g = TW_FontCreateGlyph(fa, i);
      DL_APPEND(fa->glyphs, g);
    }
  }

  return fa;
}
