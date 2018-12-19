#ifndef CENTER_H
#define CENTER_H

#include "../core/component.h"

typedef struct s_center {
#include "../core/component_props.inc"
} center;

int center_update(component *);

#define CENTER_DEFAULTS                                                        \
  .update = &center_update, .render = component_render_children

#define CENTER(...)                                                            \
  ((component *)COMPONENT(center, CENTER_DEFAULTS, __VA_ARGS__))

#endif