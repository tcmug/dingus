
#include "text.h"
#include "../core/log.h"
#include "../core/texture.h"

font_atlas *default_font;

int text_update(TW_Component *_self) {
  /*
    text *self = (text *)_self;

    // Mark resized if text change causes size change.
    TW_Rectangle new_rect = _self->rect;
    print_size(default_font, self->text, &new_rect);

    if (new_rect.w != _self->rect.w || new_rect.h != _self->rect.h)
      self->resized = 1;

    self->rect = new_rect;

    if (self->resized) {
      if (self->cache) {
        TW_TextureDestroy(self->cache);
      }
      self->resized = 0;
      self->cache = TW_TextureRenderTarget(self->rect.w, self->rect.h);
      app_log("Resized TW_Texture x");
    }

    SDL_Rect rect = {0, self->rect.h, self->rect.w, self->rect.h};
    TW_TextureStartRender(self->cache);
    TW_MatrixGLUniform("projection",
                       TW_MatrixOrthogonalProjection(0, self->cache->width, 0,
                                                     self->cache->height, 0,
    1)); glClearColor(0, 0, 0.6, 0); glClear(GL_COLOR_BUFFER_BIT);
    print_rect(default_font, rect, self->text);
    TW_TextureEndRender(self->cache);*/
  return 0;
}

void TW_TextRender(const TW_Component *parent, TW_Component *_self) {
  TW_Window *props = (TW_Window *)_self->TW_Window;
  TW_Text *self = (TW_Text *)_self;
  SDL_Rect rect = {self->rect.x, self->rect.h - self->rect.y, self->rect.w,
                   self->rect.h};
  print_rect(default_font, rect, self->text);
  // app_log("%u %u %u %u", rect.x, rect.y, rect.w, rect.h);
}

/*
  {

      TW_ShaderUse(glyphs);
      TW_TextureStartRender(frm);

      glClearColor(0.4, 0.4, 0, 0);
      glClear(GL_COLOR_BUFFER_BIT);

      print_rect(default_font, (SDL_Rect){0, 200, 200, 200},
                 L"Hello World, what is going on in Japan (日本)? I hope "
                 L"things are going well, because if they were not going"
                 L"well things would not be going ok, they would be going"
                 L"rather badly, is not to say that bad is not good, but"
                 L"inheritly it is.");

      TW_TextureEndRender(frm);
    }
*/