
#include "../core/log.h"
#include "../core/texture.h"

#include "view.h"

void TW_ComponentViewRender(const TW_Component *parent, TW_Component *_self) {

  TW_Window *props = (TW_Window *)_self->window;
  TW_ComponentView *self = (TW_ComponentView *)_self;

  if (self->resized) {
    if (self->cache) {
      TW_TextureFree(self->cache);
    }
    self->rerender = 1;
    self->resized = 0;
    self->cache =
        TW_TextureRenderTarget(self->rect.w, self->rect.h, self->hasDepth);
  }

  if (self->rerender) {
    self->rerender = 0;
    TW_TextureStartRender(self->cache);
    glClearColor(self->color[0], self->color[1], self->color[2],
                 self->color[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    TW_MatrixGLUniform(
        "projection",
        TW_MatrixOrthogonalProjection(0, self->rect.w, 0, self->rect.h, 0, 1));
    TW_ComponentRenderChildren(_self);
    TW_TextureEndRender(self->cache);
  }

  // FIXME: Silly way of determining rendering area projection, instead there
  // should always be a parent, a root
  if (parent) {
    TW_MatrixGLUniform("projection",
                       TW_MatrixOrthogonalProjection(0, parent->rect.w, 0,
                                                     parent->rect.h, 0, 1));
  } else {
    app_log("failing...");
    TW_MatrixGLUniform(
        "projection",
        TW_MatrixOrthogonalProjection(0, self->rect.w, 0, self->rect.h, 0, 1));
  }
  TW_TextureDraw(self->cache, self->rect);
}