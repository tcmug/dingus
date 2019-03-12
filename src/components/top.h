#ifndef TOP_H
#define TOP_H

#include "../core/ui.h"

void TW_ComponentTopRender(const TW_Component *parent, TW_Component *_self);
void TW_ComponentTopRightRender(const TW_Component *parent,
                                TW_Component *_self);

#define TOP_DEFAULTS .render = TW_ComponentTopRender
#define TOP_RIGHT_DEFAULTS .render = TW_ComponentTopRightRender

#define TOP(...)                                                               \
  ((TW_Component *)TW_Component(TW_Component, TOP_DEFAULTS, __VA_ARGS__))

#define TOP_RIGHT(...)                                                         \
  ((TW_Component *)TW_Component(TW_Component, TOP_RIGHT_DEFAULTS, __VA_ARGS__))

#endif