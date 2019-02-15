
#include "../core/log.h"

#include "center.h"

int center_update(TW_Component *_self) {

  if (!_self->children)
    return 0;

  int children_height = 0;

  for (int i = 0; _self->children[i]; i++)
    if (_self->children[i]->render) {
      children_height += _self->children[i]->rect.h;
    }

  int y = (_self->rect.y + _self->rect.h - children_height) / 2;

  for (int i = 0; _self->children[i]; i++)
    if (_self->children[i]->render) {
      TW_Component *child = _self->children[i];
      child->rect.x = (_self->rect.x + _self->rect.w - child->rect.w) / 2;
      child->rect.y = y;
      y += child->rect.h;
    }
  return 0;
}
