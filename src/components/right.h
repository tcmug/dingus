#ifndef RIGHT_H
#define RIGHT_H

#include "../core/ui.h"

typedef struct right_t {
#include "../core/ui/component_props.inc"
} right;

void TW_ComponentRightRender(TW_Component *);

#define RIGHT_DEFAULTS .render = TW_ComponentRightRender

#define RIGHT(...)                                                             \
  ((TW_Component *)TW_Component(right, RIGHT_DEFAULTS, __VA_ARGS__))

#endif