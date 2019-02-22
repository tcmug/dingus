#ifndef VIEW_H
#define VIEW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../core/component.h"
#include "../core/print.h"

extern font_atlas *default_font;

typedef struct TW_ComponentView_t {
#include "../core/component_props.inc"
  real color[4];
} TW_ComponentView;

void TW_ComponentViewRender(const TW_Component *parent, TW_Component *);

#define VIEW_DEFAULTS                                                          \
  .render = &TW_ComponentViewRender, .rerender = 1, .color = {1, 1, 1, 0}

#define VIEW(...)                                                              \
  ((TW_Component *)TW_Component(TW_ComponentView, VIEW_DEFAULTS, __VA_ARGS__))

#endif