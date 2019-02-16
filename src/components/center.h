#ifndef CENTER_H
#define CENTER_H

#include "../core/component.h"

typedef struct center_t {
#include "../core/component_props.inc"
} center;

int center_update(TW_Component *);

#define CENTER_DEFAULTS .render = TW_ComponentRenderChildren

#define CENTER(...)                                                            \
  ((TW_Component *)TW_Component(center, CENTER_DEFAULTS, __VA_ARGS__))

#endif