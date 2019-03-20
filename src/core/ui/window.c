
#include "window.h"
#include "../shader.h"

void TW_WindowRender(TW_Window *window) {
  TW_ShaderUse(window->shader);
  glViewport(0, 0, window->width, window->height);
  glClearColor(0.2, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);
  window->rect = (TW_Rectangle){0, 0, window->width, window->height};
  TW_ComponentRenderChildren((TW_Component *)window);
}