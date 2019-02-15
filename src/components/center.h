#ifndef CENTER_H
#define CENTER_H

#include "../core/component.h"

typedef struct center_t {
#include "../core/component_props.inc"
} center;

int center_update(TW_Component *);

#define CENTER_DEFAULTS                                                        \
  .update = &center_update, .render = component_render_children

#define CENTER(...)                                                            \
  ((TW_Component *)TW_Component(center, CENTER_DEFAULTS, __VA_ARGS__))

#endif