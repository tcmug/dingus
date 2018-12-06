
#include "../core/log.h"

#include "text.h"

font_atlas *default_font;

int text_update(component *_self) {
  text *self = (text *)_self;
  window *props = (window *)_self->window;

  if (self->resized) {
    if (self->texture) {
      SDL_DestroyTexture(self->texture);
    }
    self->resized = 0;
    self->texture =
        SDL_CreateTexture(props->renderer, SDL_PIXELFORMAT_RGBA8888,
                          SDL_TEXTUREACCESS_TARGET, self->rect.w, self->rect.h);
    app_debug("Resized texture");
  }
  SDL_SetRenderTarget(props->renderer, self->texture);
  SDL_RenderClear(props->renderer);
  SDL_Rect rect = {0, 0, self->rect.w, self->rect.h};
  print_rect(props->renderer, default_font, rect, self->text);
  SDL_SetRenderTarget(props->renderer, 0);
  return 0;
}

void texture_render(component *self) {
  window *props = (window *)self->window;
  if (self->texture) {
    // SDL_SetRenderTarget(props->renderer, self->texture);
    // SDL_Rect dest = {self->rect.w / 4, self->rect.h / 4, self->rect.w,
    //                 self->rect.h};
    // SDL_RenderCopy(props->renderer, self->texture, 0, &dest);
    // SDL_SetRenderTarget(props->renderer, 0);
    SDL_RenderCopy(props->renderer, self->texture, 0, &self->rect);
  }
}

SDL_Texture *drawText(SDL_Renderer *renderer, TTF_Font *font,
                      const char *text) {

  SDL_Color White = {255, 255, 255};
  SDL_Surface *surface = TTF_RenderText_Solid(font, text, White);

  SDL_Texture *Message = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  return Message;
}
