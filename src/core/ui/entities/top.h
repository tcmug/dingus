#ifndef TOP_H
#define TOP_H

#include "../../ui.h"

void TW_EntityTopRender(TW_Entity *_self);
void TW_EntityTopRightRender(TW_Entity *_self);

#define TOP_DEFAULTS .render = TW_EntityTopRender
#define TOP_RIGHT_DEFAULTS .render = TW_EntityTopRightRender

#define TOP(...) ((TW_Entity *)TW_Entity(TW_Entity, TOP_DEFAULTS, __VA_ARGS__))

#define TOP_RIGHT(...)                                                         \
  ((TW_Entity *)TW_Entity(TW_Entity, TOP_RIGHT_DEFAULTS, __VA_ARGS__))

#endif