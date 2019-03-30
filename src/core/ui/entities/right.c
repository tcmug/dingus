#include "right.h"
#include "../../log.h"
#include "utlist.h"

void TW_EntityRightRender(TW_Entity *_self) {

  if (!_self->kids || !_self->parent)
    return;

  _self->rect = _self->parent->rect;

  TW_Entity *kid;
  DL_FOREACH(_self->kids, kid) { kid->rect.x = _self->rect.w - kid->rect.w; }
  TW_EntityRenderChildren(_self);
}
