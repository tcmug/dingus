
#include "../core/log.h"

#include "right.h"

void TW_ComponentRightRender(const TW_Component *parent, TW_Component *_self) {

  if (!_self->children || !parent)
    return;

  _self->rect = parent->rect;

  for (int i = 0; _self->children[i]; i++)
    if (_self->children[i]->render) {
      TW_Component *child = _self->children[i];
      child->rect.x = _self->rect.w - child->rect.w;
    }
  TW_ComponentRenderChildren(_self);
}
