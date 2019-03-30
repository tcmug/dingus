#ifndef VIEW_H
#define VIEW_H

#include "../../ui.h"

extern TW_FontAtlas *default_font;

typedef struct TW_EntityView_t {
#include "../entity_props.inc"
  real color[4];
  int hasDepth;
} TW_EntityView;

void TW_EntityViewRender(TW_Entity *);

#define VIEW_DEFAULTS                                                          \
  .render = &TW_EntityViewRender, .rerender = 1, .color = {1, 1, 1, 0},        \
  .hasDepth = 0

#define VIEW(...)                                                              \
  ((TW_Entity *)TW_Entity(TW_EntityView, VIEW_DEFAULTS, __VA_ARGS__))

#endif