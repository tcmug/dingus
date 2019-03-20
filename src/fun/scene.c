
#include "scene.h"

void TW_SceneViewRender(TW_Component *_self) {

  TW_SceneView *self = (TW_SceneView *)_self;
  TW_Window *window = (TW_Window *)TW_GetRoot(_self);

  real ratio = window->rect.w / window->rect.h;
  self->projection = TW_MatrixPerspectiveProjection(0.01, 500, 70, ratio);

  if (window->cursor_mode == CURSOR_VECTOR) {
    TW_Matrix rot = TW_MatrixRotation(window->cursor_vector.y * 0.005,
                                      window->cursor_vector.x * 0.005, 0);
    self->view = TW_MatrixMulMatrix(rot, self->view);
    window->cursor_vector = (TW_Vector2){0, 0};
  }

  TW_Object *obj;
  DL_FOREACH(self->objects, obj) {
    TW_MatrixGLUniform("projection", self->projection);
    TW_MatrixGLUniform("view", self->view);
    TW_ShaderUse(obj->shader);
    TW_ObjectRender(self, obj);
  }
  /*
    TW_ShaderUse(flat);

    TW_MatrixGLUniform("projection", projection);
    TW_MatrixGLUniform("view", view);
    TW_MatrixGLUniform("model", TW_MatrixIdentity());

    if (obj) {
      TW_Matrix rot = TW_MatrixRotation(0.003, 0.005, 0.001);
      obj->orientation = TW_MatrixMulMatrix(rot, obj->orientation);
      obj->position.x = 0;
      obj->position.z = -3;
      obj->position.y = -1.6;
      TW_ObjectRender(obj);

      TW_ShaderUse(edges);

      TW_MatrixGLUniform("projection", projection);
      TW_MatrixGLUniform("view", view);
      TW_MatrixGLUniform("model", TW_MatrixIdentity());

      glFrontFace(GL_CCW);
      TW_ObjectRender(obj);
      glFrontFace(GL_CW);
    }

    TW_ShaderUse(uishader);
    */

  TW_ComponentRerender(_self);

  TW_ShaderUse(window->shader);
}

int l_SCENE(lua_State *L) {

  TW_SceneView *comp = (TW_SceneView *)SCENE();
  lua_pushstring(L, "objects");
  lua_gettable(L, -2);
  if (lua_istable(L, -1)) {
    lua_pushnil(L);
    while (lua_next(L, -2) != 0) {
      if (lua_islightuserdata(L, -1)) {
        TW_Object *obj = (TW_Object *)lua_touserdata(L, -1);
        DL_APPEND(comp->objects, obj);
        app_log("Added object %p %f %f %f", obj, obj->position.x,
                obj->position.y, obj->position.z);
      }
      lua_pop(L, 1);
    }
  }

  l_prep_kids(L, (TW_Component *)comp);

  comp->projection = TW_MatrixIdentity();
  comp->view = TW_MatrixIdentity();

  lua_pushlightuserdata(L, comp);
  return 1;
}