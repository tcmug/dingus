
#include "../../log.h"
#include "../../resources/shader.h"
#include "../../resources/texture.h"

#include "view.h"

void TW_EntityViewRender(TW_Entity *_self) {

  TW_EntityView *self = (TW_EntityView *)_self;

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
    if (self->hasDepth)
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    else
      glClear(GL_COLOR_BUFFER_BIT);
    TW_MatrixGLUniform(
        "projection",
        TW_MatrixOrthogonalProjection(0, self->rect.w, 0, self->rect.h, 0, 1));
    TW_EntityRenderChildren(_self);
    TW_TextureEndRender(self->cache);
  }

  TW_Window *window = (TW_Window *)TW_GetRoot(_self);
  TW_ShaderUse(window->shader);
  TW_Vector3GLUniform("tint", (TW_Vector3){1, 1, 1});
  TW_MatrixGLUniform(
      "projection",
      TW_MatrixOrthogonalProjection(0, _self->rect.w, 0, _self->rect.h, 0, 1));
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  TW_TextureDraw(self->cache, self->rect);
  glDisable(GL_BLEND);
}