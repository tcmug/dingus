#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <stdlib.h>

#include "engine.h"
#include "log.h"

typedef struct shader_t {
  GLuint program;
  GLuint geometry;
  GLuint vertex;
  GLuint fragment;
} TW_Shader;

TW_Shader TW_ShaderLoad(const char *vertex_name, const char *geometry_name,
                        const char *fragment_name);
void TW_ShaderUse(TW_Shader s);

#endif