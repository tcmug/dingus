#ifndef VIEW_H
#define VIEW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../core/component.h"
#include "../core/print.h"

extern font_atlas *default_font;

typedef struct TW_ComponentView_t {
#include "../core/component_props.inc"
  int rerender;
} TW_ComponentView;

int TW_ComponentViewUpdate(TW_Component *);
void TW_ComponentViewRender(TW_Component *);

#define VIEW_DEFAULTS                                                          \
  .update = &TW_ComponentViewUpdate, .render = &TW_ComponentViewRender,        \
  .rerender = 1

#define VIEW(...)                                                              \
  ((TW_Component *)TW_Component(TW_ComponentView, VIEW_DEFAULTS, __VA_ARGS__))

#endif