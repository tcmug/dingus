
#include "text.h"
#include "../core/log.h"
#include "../core/texture.h"

font_atlas *default_font;

void TW_TextRender(const TW_Component *parent, TW_Component *_self) {
  TW_Window *props = (TW_Window *)_self->window;
  TW_Text *self = (TW_Text *)_self;
  SDL_Rect rect = {self->rect.x, self->rect.y, self->rect.w, self->rect.h};
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  TW_Vector3GLUniform("tint", self->color);
  print_rect(default_font, rect, self->text);
  TW_Vector3GLUniform("tint", (TW_Vector3){1, 1, 1});
  glDisable(GL_BLEND);
}
