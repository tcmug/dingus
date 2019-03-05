
#include "top.h"

void TW_ComponentTopRender(const TW_Component *parent, TW_Component *_self) {

  if (!_self->kids || !parent)
    return;

  _self->rect = parent->rect;

  TW_Component *kid;
  DL_FOREACH(_self->kids, kid)
  if (kid->render) {
    kid->rect.y = _self->rect.y + _self->rect.h;
  }

  TW_ComponentRenderChildren(_self);
}

void TW_ComponentTopRightRender(const TW_Component *parent,
                                TW_Component *_self) {

  if (!_self->kids || !parent)
    return;

  _self->rect = parent->rect;

  TW_Component *kid;
  DL_FOREACH(_self->kids, kid)
  if (kid->render) {
    kid->rect.x = _self->rect.w - kid->rect.w;
    kid->rect.y = _self->rect.y + _self->rect.h;
  }

  TW_ComponentRenderChildren(_self);
}
