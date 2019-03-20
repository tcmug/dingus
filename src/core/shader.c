
#include "shader.h"
#include "resources.h"

TW_Shader *shaders = 0;

GLuint _shader_load(const char *filename, int type);

char *str_malloc(const char *str) {
  if (!str) {
    return 0;
  }
  int len = strlen(str) + 1;
  char *dest = malloc(len);
  memcpy(dest, str, len);
  return dest;
}

void TW_ShaderReload(TW_Shader *s) {
  if (s->fragment)
    glDeleteShader(s->fragment);
  if (s->geometry)
    glDeleteShader(s->geometry);
  if (s->vertex)
    glDeleteShader(s->vertex);
  if (s->program)
    glDeleteProgram(s->program);

  s->program = glCreateProgram();
  engine_gl_check();

  if (s->vertex_src) {
    s->vertex = _shader_load(s->vertex_src, GL_VERTEX_SHADER);
    if (s->vertex) {
      glAttachShader(s->program, s->vertex);
      engine_gl_check();
    }
  }

  if (s->geometry_src) {
    s->geometry = _shader_load(s->geometry_src, GL_GEOMETRY_SHADER);
    if (s->geometry) {
      glAttachShader(s->program, s->geometry);
      engine_gl_check();
    }
  }

  if (s->fragment_src) {
    s->fragment = _shader_load(s->fragment_src, GL_FRAGMENT_SHADER);
    if (s->fragment) {
      glAttachShader(s->program, s->fragment);
      engine_gl_check();
    }
  }

  glLinkProgram(s->program);

  GLint success = 0;
  glGetProgramiv(s->program, GL_LINK_STATUS, &success);

  if (success == GL_FALSE) {
    // We failed to compile.

    app_warning("Shader linking failed.");

    // GLint maxLength = 0;
    // glGetShaderiv(s, GL_INFO_LOG_LENGTH, &maxLength);

    int len = 400000;
    char msg[len];

    glGetProgramInfoLog(s->program, len, 0, msg);
    app_warning(msg);
  }

  engine_gl_check();
  app_log("Program successfully linked.");
}

TW_Shader *TW_ShaderLoad(const char *vertex_name, const char *geometry_name,
                         const char *fragment_name) {

  char name[1024];
  sprintf(name, "%s,%s,%s", vertex_name, geometry_name, fragment_name);
  TW_Shader *s = (TW_Shader *)TW_ResourceFind((TW_Resource *)shaders, name);
  if (s) {
    printf("Shader loaded from cache (%s)\n", name);
    return s;
  }

  s = (TW_Shader *)malloc(sizeof(TW_Shader));
  memset(s, 0, sizeof(TW_Shader));

  s->fragment_src = str_malloc(fragment_name);
  s->geometry_src = str_malloc(geometry_name);
  s->vertex_src = str_malloc(vertex_name);

  TW_ShaderReload(s);

  shaders = (TW_Shader *)TW_ResourceAdd((TW_Resource *)shaders, name,
                                        (TW_Resource *)s);
  return s;
}

void TW_ShaderFree(TW_Shader *s) {
  if (s->fragment)
    glDeleteShader(s->fragment);
  if (s->geometry)
    glDeleteShader(s->geometry);
  if (s->vertex)
    glDeleteShader(s->vertex);
  if (s->program)
    glDeleteProgram(s->program);
  free(s);
  app_log("Shader freed");
}

GLuint _shader_load(const char *filename, int type) {

  FILE *f = fopen(filename, "rb");
  if (!f) {
    app_warning("TW_Shader file \"%s\" not found.", filename);
    return 0;
  }
  fseek(f, 0, SEEK_END);
  int size = ftell(f);
  fseek(f, 0, SEEK_SET);

  if (!(size > 0)) {
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
    int len = 400000;
    char msg[len];
    glGetShaderInfoLog(s, len, 0, msg);
    app_warning(msg);
    glDeleteShader(s);
    return 0;
  }

  engine_gl_check();
  app_log("Shader \"%s\" compiled.", filename);
  return s;
}

void TW_ShaderUse(TW_Shader *s) {
  glUseProgram(s->program);
  engine_gl_check();
}

GLint TW_ShaderGLUniformLoc(TW_Shader *s, const char *name) {
  return glGetUniformLocation(s->program, name);
}

int l_SHADER(lua_State *L) {
  const char *vert = lua_get_table_string(L, "vert", 0);
  const char *geo = lua_get_table_string(L, "geo", 0);
  const char *frag = lua_get_table_string(L, "frag", 0);
  char fragstr[256], geostr[256], vertstr[256];
  if (vert) {
    sprintf(vertstr, RESOURCE("share/dingus/shaders/%s"), vert);
    vert = vertstr;
  }
  if (geo) {
    sprintf(geostr, RESOURCE("share/dingus/shaders/%s"), geo);
    geo = geostr;
  }
  if (frag) {
    sprintf(fragstr, RESOURCE("share/dingus/shaders/%s"), frag);
    frag = fragstr;
  }
  lua_pushlightuserdata(L, TW_ShaderLoad(vert, geo, frag));
  return 1;
}

void TW_ShaderReloadAll() {
  TW_Resource *shader;
  LL_FOREACH((TW_Resource *)shaders, shader)
  TW_ShaderReload((TW_Shader *)shader);
}