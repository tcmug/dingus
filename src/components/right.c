
#include "right.h"
#include "../core/log.h"
#include "utlist.h"

void TW_ComponentRightRender(TW_Component *_self) {

  if (!_self->kids || !_self->parent)
    return;

  _self->rect = _self->parent->rect;

  TW_Component *kid;
  DL_FOREACH(_self->kids, kid) { kid->rect.x = _self->rect.w - kid->rect.w; }
  TW_ComponentRenderChildren(_self);
}
