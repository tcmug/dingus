
#include "top.h"

void TW_ComponentTopRender(TW_Component *_self) {

  if (!_self->kids || !_self->parent)
    return;

  _self->rect = _self->parent->rect;

  TW_Component *kid;
  DL_FOREACH(_self->kids, kid)
  if (kid->render) {
    kid->rect.y = _self->rect.y + _self->rect.h;
  }

  TW_ComponentRenderChildren(_self);
}

void TW_ComponentTopRightRender(TW_Component *_self) {

  if (!_self->kids || !_self->parent)
    return;

  _self->rect = _self->parent->rect;

  TW_Component *kid;
  DL_FOREACH(_self->kids, kid)
  if (kid->render) {
    kid->rect.x = _self->rect.w - kid->rect.w;
    kid->rect.y = _self->rect.y + _self->rect.h;
  }

  TW_ComponentRenderChildren(_self);
}
