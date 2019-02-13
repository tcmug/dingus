
#include "text.h"
#include "../core/log.h"
#include "../core/texture.h"

font_atlas *default_font;

int text_update(component *_self) {
  text *self = (text *)_self;
  window *props = (window *)_self->window;

  // Mark resized if text change causes size change.
  rectangle new_rect = _self->rect;
  print_size(default_font, self->text, &new_rect);

  if (new_rect.w != _self->rect.w || new_rect.h != _self->rect.h)
    self->resized = 1;

  self->rect = new_rect;

  if (self->resized) {
    if (self->cache) {
      texture_destroy(self->cache);
    }
    self->resized = 0;
    self->cache = texture_render_target(self->rect.w, self->rect.h);
    app_debug("Resized texture");
  }

  // SDL_SetRenderTarget(props->renderer, self->texture);
  // SDL_SetRenderDrawColor(props->renderer, self->background.r,
  //                        self->background.g, self->background.b,
  //                        self->background.a);
  // SDL_RenderClear(props->renderer);
  SDL_Rect rect = {0, 0, self->rect.w, self->rect.h};

  // SDL_SetTextureColorMod(self->texture, self->color.r, self->color.g,
  //                        self->color.b);
  // SDL_SetTextureAlphaMod(self->texture, self->color.a);
  print_rect(default_font, rect, self->text);

  // SDL_SetRenderTarget(props->renderer, 0);
  return 0;
}

void texture_render(component *_self) {
  window *props = (window *)_self->window;
  text *self = (text *)_self;
  if (self->cache) {
    // SDL_RenderCopy(props->renderer, self->texture, 0, &self->rect);
  }
}
