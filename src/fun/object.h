#ifndef TW_OBJECT_H
#define TW_OBJECT_H

#include "../core/buffer.h"
#include "../core/math.h"
#include "../core/resources.h"
#include "scene.h"

#include <config.h>

#include <lua.h>

typedef struct TW_Model_t {
#include "../core/resource_props.h"
  TW_Vector3Buffer vertices;
  TW_Vector3Buffer normals;
  TW_Vector2Buffer uvs;
  TW_IndicesBuffer indices;
} TW_Model;

typedef struct TW_Object_t {
  TW_Vector3 position;
  TW_Matrix orientation;
  TW_Model *model;
  TW_Shader *shader;

  struct TW_Object_t *kids;
  struct TW_Object_t *prev;
  struct TW_Object_t *next;

} TW_Object;

TW_Object *TW_ObjectCreate(TW_Vector3 position, TW_Model *model);
void TW_ObjectFree(TW_Object *object);
void TW_ObjectRender(TW_SceneView *scene, TW_Object *object);

int l_MODEL(lua_State *L);
int l_OBJECT(lua_State *L);

#endif