#include "center.h"
#include "../core/log.h"
#include "utlist.h"

int center_update(TW_Component *_self) {

  if (!_self->kids)
    return 0;

  int children_height = 0;

  TW_Component *kid;
  DL_FOREACH(_self->kids, kid) {
    if (kid->render)
      children_height += kid->rect.h;
  }

  int y = (_self->rect.y + _self->rect.h - children_height) / 2;

  DL_FOREACH(_self->kids, kid) {
    if (kid->render) {
      kid->rect.x = (_self->rect.x + _self->rect.w - kid->rect.w) / 2;
      kid->rect.y = y;
      y += kid->rect.h;
    }
  }
  return 0;
}
