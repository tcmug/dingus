
#include "../core/log.h"

#include "center.h"

int center_update(component *_self) { return 0; }

void center_render(component *_self) {

  if (!_self->children)
    return;

  window *props = (window *)_self->window;
  SDL_Rect old;
  SDL_RenderGetViewport(props->renderer, &old);

  int children_height = 0;

  for (int i = 0; _self->children[i]; i++)
    if (_self->children[i]->render) {
      children_height += _self->children[i]->rect.h;
    }

  int y = (_self->rect.y + _self->rect.h - children_height) / 2;

  for (int i = 0; _self->children[i]; i++)
    if (_self->children[i]->render) {
      component *child = _self->children[i];
      SDL_Rect centered = {(_self->rect.x + _self->rect.w - child->rect.w) / 2,
                           y, child->rect.w, child->rect.h};
      SDL_RenderSetViewport(props->renderer, &centered);
      y += child->rect.h;
      _self->children[i]->render(_self->children[i]);
    }

  SDL_RenderSetViewport(props->renderer, &old);
}
