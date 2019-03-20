

#include "object.h"

#include "utlist.h"

TW_Model *TW_ModelLoad(const char *name);
TW_Model *TW_ModelSmoothen(const TW_Model *);

TW_Object *TW_ObjectCreate(TW_Vector3 position, TW_Model *model) {

  TW_Object *obj = (TW_Object *)malloc(sizeof(TW_Object));

  obj->orientation = TW_MatrixIdentity();
  obj->position = position;
  obj->model = model;
  // TW_ModelSmoothen(TW_ModelLoad(name));

  obj->shader = 0;
  obj->kids = 0;
  obj->next = 0;
  obj->prev = 0;

  return obj;
}

void TW_ObjectAddChild(TW_Object *self, TW_Object *child) {
  DL_APPEND(self->kids, child);
}

void TW_ObjectFree(TW_Object *object) { free(object); }

void TW_ObjectRender(TW_SceneView *scene, TW_Object *self) {

  if (!self->shader || !self->model) {
    app_log("Shader or model missing :(");
    return;
  }

  // TW_Matrix temp = TW_MatrixTranslation(TW_MatrixIdentity(),
  // self->position); TW_Matrix result =
  // TW_MatrixMulMatrix(self->orientation, temp);
  self->orientation.m = self->position.x;
  self->orientation.n = self->position.y;
  self->orientation.o = self->position.z;

  // app_log("%f %f %f", self->position.x, self->position.y, self->position.z);
  // TW_ShaderUse(self->shader);

  TW_MatrixGLUniform("model", self->orientation);

  TW_Vector3BufferBind(&(self->model->vertices), 0);
  TW_Vector3BufferBind(&(self->model->normals), 1);
  TW_Vector2BufferBind(&(self->model->uvs), 2);
  TW_IndicesBufferBind(&(self->model->indices));

  glDrawElements(GL_TRIANGLES,              // mode
                 self->model->indices.size, // count
                 GL_UNSIGNED_INT,           // type
                 (void *)0);

  TW_Object *kid;
  DL_FOREACH(self->kids, kid) TW_ObjectRender(scene, kid);
}

int l_MODEL(lua_State *L) {
  const char *model = lua_get_table_string(L, "file", 0);
  char modelstr[256];
  if (model) {
    sprintf(modelstr, RESOURCE("share/dingus/models/%s"), model);
    lua_pushlightuserdata(L, TW_ModelLoad(modelstr));
  } else {
    lua_pushnil(L);
  }
  return 1;
}

int l_OBJECT(lua_State *L) {
  TW_Model *model = lua_get_table_userdata(L, "model", 0);
  TW_Shader *shader = lua_get_table_userdata(L, "shader", 0);
  TW_Vector3 p = {lua_get_table_int(L, "x", 0), lua_get_table_int(L, "y", 0),
                  lua_get_table_int(L, "z", 0)};
  TW_Object *obj = TW_ObjectCreate(p, model);
  obj->shader = shader;
  obj->kids = 0;
  lua_pushstring(L, "children");
  lua_gettable(L, -2);
  if (lua_istable(L, -1)) {
    lua_pushnil(L);
    while (lua_next(L, -2) != 0) {
      if (lua_islightuserdata(L, -1)) {
        TW_ObjectAddChild(obj, (TW_Object *)lua_touserdata(L, -1));
      }
      lua_pop(L, 1);
    }
  }
  lua_pop(L, 1);

  lua_pushlightuserdata(L, obj);
  return 1;
}