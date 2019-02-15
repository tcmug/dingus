
#include "../core/log.h"

#include "right.h"

int right_update(TW_Component *_self) {

  if (!_self->children)
    return 0;

  for (int i = 0; _self->children[i]; i++)
    if (_self->children[i]->render) {
      TW_Component *child = _self->children[i];
      child->rect.x = _self->rect.w - child->rect.w;
    }
  return 0;
}
