#ifndef TEXT_H
#define TEXT_H

#include "../core/component.h"
#include "../core/print.h"

extern font_atlas *default_font;

typedef struct s_text_state {
  const wchar_t *text;
} text_state;

typedef struct s_text {
#include "../core/component_props.h"
  text_state state;
} text;

int text_update(component *);
void texture_render(component *);
SDL_Texture *drawText(SDL_Renderer *renderer, TTF_Font *font, const char *text);

#define TEXT_DEFAULTS .update = &text_update, .render = &texture_render

#define TEXT(...) COMPONENT_ALLOC(text, TEXT_DEFAULTS, __VA_ARGS__)

#endif