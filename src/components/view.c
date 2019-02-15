
#include "../core/log.h"
#include "../core/texture.h"

#include "view.h"
/*

screen (passthrough rendering)
  buffer (3d)
    kids ->
      (ui comps)
      buffer (3d another vp?)


*/

int TW_ComponentViewUpdate(TW_Component *_self) {
  TW_Window *props = (TW_Window *)_self->TW_Window;
  TW_ComponentView *self = (TW_ComponentView *)_self;

  if (self->resized) {
    if (self->cache) {
      TW_TextureDestroy(self->cache);
    }
    self->resized = 0;
    self->cache = TW_TextureRenderTarget(self->rect.w, self->rect.h);
    app_log("View resized");
  }
}

#include "../core/shader.h"
extern TW_Shader uishader;

void TW_ComponentViewRender(TW_Component *_self) {

  TW_Window *props = (TW_Window *)_self->TW_Window;
  TW_ComponentView *self = (TW_ComponentView *)_self;

  if (self->resized) {
    if (self->cache) {
      TW_TextureDestroy(self->cache);
    }
    self->rerender = 1;
    self->resized = 0;
    self->cache = TW_TextureRenderTarget(self->rect.w - self->rect.x,
                                         self->rect.h - self->rect.y);
  }

  if (self->rerender) {
    TW_TextureStartRender(self->cache);
    glClearColor(0, 0.5, 0.6, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    TW_MatrixGLUniform(
        "projection",
        TW_MatrixOrthogonalProjection(0, self->rect.w, 0, self->rect.h, 0, 1));
    component_render_children(_self);
    TW_TextureEndRender(self->cache);
  }

  GLuint program;
  TW_ShaderUse(uishader);
  glGetIntegerv(GL_CURRENT_PROGRAM, &program);
  glUniform1i(glGetUniformLocation(program, "surface"), 0);
  TW_MatrixGLUniform("projection", TW_MatrixOrthogonalProjection(
                                       0, self->rect.w, 0, self->rect.h, 0, 1));

  TW_TextureDraw(self->cache, self->rect);
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