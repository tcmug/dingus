
#include "right.h"
#include "../core/log.h"
#include "utlist.h"

void TW_ComponentRightRender(const TW_Component *parent, TW_Component *_self) {

  if (!_self->kids || !parent)
    return;

  _self->rect = parent->rect;

  TW_Component *kid;
  DL_FOREACH(_self->kids, kid) { kid->rect.x = _self->rect.w - kid->rect.w; }
  TW_ComponentRenderChildren(_self);
}
