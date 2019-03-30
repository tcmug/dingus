
#include "top.h"

void TW_EntityTopRender(TW_Entity *_self) {

  if (!_self->kids || !_self->parent)
    return;

  _self->rect = _self->parent->rect;

  TW_Entity *kid;
  DL_FOREACH(_self->kids, kid)
  if (kid->render) {
    kid->rect.y = _self->rect.y + _self->rect.h;
  }

  TW_EntityRenderChildren(_self);
}

void TW_EntityTopRightRender(TW_Entity *_self) {

  if (!_self->kids || !_self->parent)
    return;

  _self->rect = _self->parent->rect;

  TW_Entity *kid;
  DL_FOREACH(_self->kids, kid)
  if (kid->render) {
    kid->rect.x = _self->rect.w - kid->rect.w;
    kid->rect.y = _self->rect.y + _self->rect.h;
  }

  TW_EntityRenderChildren(_self);
}
