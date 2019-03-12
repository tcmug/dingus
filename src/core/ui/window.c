
#include "window.h"

void TW_WindowRender(TW_Window *window) {
  glViewport(0, 0, window->width, window->height);
  glClearColor(0.2, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  window->rect = (TW_Rectangle){0, 0, window->width, window->height};
  TW_ComponentRenderChildren((TW_Component *)window);
}