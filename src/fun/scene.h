#ifndef TW_SCENE_H
#define TW_SCENE_H

#include "../core/ui.h"

extern TW_FontAtlas *default_font;

typedef struct TW_SceneView_t {
#include "../core/ui/component_props.inc"

  struct TW_Object_t *objects;
  TW_Matrix view;
  TW_Matrix projection;

} TW_SceneView;

#include "object.h"

void TW_SceneViewRender(TW_Component *);

#define SCENE_DEFAULTS .render = TW_SceneViewRender, .objects = 0

#define SCENE(...)                                                             \
  ((TW_Component *)TW_Component(TW_SceneView, SCENE_DEFAULTS, __VA_ARGS__))

int l_SCENE(lua_State *L);

#endif