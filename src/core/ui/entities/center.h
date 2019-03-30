#ifndef CENTER_H
#define CENTER_H

#include "../../ui.h"

typedef struct center_t {
#include "../entity_props.inc"
} center;

int center_update(TW_Entity *);

#define CENTER_DEFAULTS .render = TW_EntityRenderChildren

#define CENTER(...)                                                            \
  ((TW_Entity *)TW_Entity(center, CENTER_DEFAULTS, __VA_ARGS__))

#endif