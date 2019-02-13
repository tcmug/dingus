#ifndef RIGHT_H
#define RIGHT_H

#include "../core/component.h"

typedef struct right_t {
#include "../core/component_props.inc"
} right;

int right_update(component *);

#define RIGHT_DEFAULTS                                                         \
  .update = &right_update, .render = component_render_children

#define RIGHT(...) ((component *)COMPONENT(right, RIGHT_DEFAULTS, __VA_ARGS__))

#endif