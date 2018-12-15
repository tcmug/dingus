#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../core/component.h"
#include "../core/print.h"

extern font_atlas *default_font;

typedef struct s_text {
#include "../core/component_props.inc"
  const wchar_t *text;
} text;

int text_update(component *);
void texture_render(component *);

#define TEXT_DEFAULTS .update = &text_update, .render = &texture_render

#define TEXT(...) ((component *)COMPONENT(text, TEXT_DEFAULTS, __VA_ARGS__))

#endif