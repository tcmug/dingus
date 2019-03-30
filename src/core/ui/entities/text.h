#ifndef TW_TEXT_H
#define TW_TEXT_H

#include "../../ui.h"

typedef char TW_Char;

extern TW_FontAtlas *default_font;

typedef struct text_t {
#include "../entity_props.inc"
  SDL_Color background;
  TW_Vector3 color;
  const TW_Char *text;
} TW_Text;

void TW_TextRender(TW_Entity *);

#define TEXT_DEFAULTS                                                          \
  .render = &TW_TextRender, .color = {1, 1, 1}, .background = {0, 0, 0, 255}

#define TEXT(...) ((TW_Entity *)TW_Entity(TW_Text, TEXT_DEFAULTS, __VA_ARGS__))

#endif