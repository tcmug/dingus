#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../core/component.h"
#include "../core/print.h"

extern font_atlas *default_font;

typedef struct text_t {
#include "../core/component_props.inc"
  SDL_Color background;
  SDL_Color color;
  const wchar_t *text;
} TW_Text;

void TW_TextRender(const TW_Component *parent, TW_Component *);

#define TEXT_DEFAULTS                                                          \
  .render = &TW_TextRender, .color = {255, 255, 255, 255},                     \
  .background = {0, 0, 0, 255}

#define TEXT(...)                                                              \
  ((TW_Component *)TW_Component(TW_Text, TEXT_DEFAULTS, __VA_ARGS__))

#endif