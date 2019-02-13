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
} shader;

GLuint _shader_load(const char *filename, int type);

shader shader_load(const char *vertex_name, const char *geometry_name,
                   const char *fragment_name) {

  shader s = {0};
  s.program = glCreateProgram();

  if (vertex_name) {
    s.vertex = _shader_load(vertex_name, GL_VERTEX_SHADER);
    if (s.vertex) {
      glAttachShader(s.program, s.vertex);
      engine_gl_check();
    }
  }

  if (geometry_name) {
    s.geometry = _shader_load(geometry_name, GL_GEOMETRY_SHADER);
    if (s.geometry) {
      glAttachShader(s.program, s.geometry);
      engine_gl_check();
    }
  }

  if (fragment_name) {
    s.fragment = _shader_load(fragment_name, GL_FRAGMENT_SHADER);
    if (s.fragment) {
      glAttachShader(s.program, s.fragment);
      engine_gl_check();
    }
  }

  glLinkProgram(s.program);

  GLint success = 0;
  glGetProgramiv(s.program, GL_LINK_STATUS, &success);

  if (success == GL_FALSE) {
    // We failed to compile.

    app_warning("Shader linking failed.");

    // GLint maxLength = 0;
    // glGetShaderiv(s, GL_INFO_LOG_LENGTH, &maxLength);

    int len = 400000;
    char msg[len];

    glGetProgramInfoLog(s.program, len, 0, msg);
    app_warning(msg);
  }

  engine_gl_check();
  app_log("Program successfully linked.");

  return s;
}

GLuint _shader_load(const char *filename, int type) {

  FILE *f = fopen(filename, "rb");
  if (!f) {
    app_warning("Shader file \"%s\" not found.", filename);
    return 0;
  }
  fseek(f, 0, SEEK_END);
  int size = ftell(f);
  fseek(f, 0, SEEK_SET);

  if (size == 0) {
    app_warning("Shader file \"%s\" is empty.", filename);
    fclose(f);
    return 0;
  }

  char *script = (char *)malloc(size + 1);
  if (!script) {
    app_warning("Out of memory while reading shader file \"%s\".", filename);
    return 0;
  }

  fread(script, 1, size, f);
  script[size] = 0;
  fclose(f);

  GLuint s = glCreateShader(type);

  const char *files[1] = {script};

  glShaderSource(s, 1, files, NULL);

  glCompileShader(s);

  engine_gl_check();

  GLint success = 0;
  glGetShaderiv(s, GL_COMPILE_STATUS, &success);

  if (success == GL_FALSE) {
    // We failed to compile.

    app_warning("Shader file \"%s\" failed compilation.", filename);

    // GLint maxLength = 0;
    // glGetShaderiv(s, GL_INFO_LOG_LENGTH, &maxLength);

    int len = 400000;
    char msg[len];

    // The maxLength includes the NULL character
    // std::TW_Vector<GLchar> errorLog(maxLength);
    glGetShaderInfoLog(s, len, 0, msg);
    app_warning(msg);
    glDeleteShader(s);
    return 0;
  }

  engine_gl_check();
  app_log("Shader \"%s\" compiled.", filename);
  return s;
}

void shader_use(shader s) {
  glUseProgram(s.program);
  engine_gl_check();
}

#endif