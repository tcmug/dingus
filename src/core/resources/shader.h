#ifndef TW_SHADER_H
#define TW_SHADER_H

#include "../engine.h"

typedef struct TW_Shader_t {
#include "resource_props.h"
  const char *vertex_src, *geometry_src, *fragment_src;
  GLuint program;
  GLuint geometry;
  GLuint vertex;
  GLuint fragment;
} TW_Shader;

void TW_ShaderReloadAll();
TW_Shader *TW_ShaderLoad(const char *vertex_name, const char *geometry_name,
                         const char *fragment_name);
void TW_ShaderFree(TW_Shader *s);
void TW_ShaderUse(TW_Shader *s);
GLint TW_ShaderGLUniformLoc(TW_Shader *s, const char *name);

int l_SHADER(lua_State *L);

#endif