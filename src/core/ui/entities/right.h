#ifndef RIGHT_H
#define RIGHT_H

#include "../../ui.h"

typedef struct right_t {
#include "../entity_props.inc"
} right;

void TW_EntityRightRender(TW_Entity *);

#define RIGHT_DEFAULTS .render = TW_EntityRightRender

#define RIGHT(...) ((TW_Entity *)TW_Entity(right, RIGHT_DEFAULTS, __VA_ARGS__))

#endif