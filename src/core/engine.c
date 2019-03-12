
#include "engine.h"
#include "log.h"

#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

GLuint global_vao = 0;

#define GL_GET(var, type, name)                                                \
  var = (type)(SDL_GL_GetProcAddress(name));                                   \
  if (!var) {                                                                  \
    printf(" - %s\n", name);                                                   \
    return props;                                                              \
  }

int lua_get_table_int(lua_State *L, const char *key, int def) {
  int value;
  lua_pushstring(L, key);
  lua_gettable(L, -2);
  if (lua_isnumber(L, -1)) {
    value = lua_tointeger(L, -1);
  } else {
    value = def;
  }
  lua_pop(L, 1);
  return value;
}

const char *lua_get_table_string(lua_State *L, const char *key,
                                 const char *def) {
  const char *value = 0;
  lua_pushstring(L, key);
  lua_gettable(L, -2);
  if (lua_isstring(L, -1)) {
    value = lua_tostring(L, -1);
  } else {
    value = def;
  }
  lua_pop(L, 1);

  return value;
}

int engine_shutdown(TW_Window *props) {
  glDeleteVertexArrays(1, &global_vao);
  SDL_DestroyWindow(props->sdl_window);
  SDL_Quit();
  lua_close(props->lua);
  free(props);
  return 0;
}

int _engine_load_config(TW_Window *props) {

  props->lua = luaL_newstate();

  luaL_openlibs(props->lua);

  if (luaL_dofile(props->lua,
                  RESOURCE("share/dingus/scripts/preferences.lua"))) {
    app_log("Couldn't load file: %s\n", lua_tostring(props->lua, -1));
    return 0;
  }

  lua_getglobal(props->lua, "preferences");
  if (lua_isnil(props->lua, -1)) {
    app_log("No preferences - using defaults.");
    lua_newtable(props->lua);
  }

  props->width = lua_get_table_int(props->lua, "width", 800);
  props->height = lua_get_table_int(props->lua, "height", 600);
  props->frame_limit = lua_get_table_int(props->lua, "frame_limit", 60);
  props->vsync = lua_get_table_int(props->lua, "vsync", 1);
  props->resizable = lua_get_table_int(props->lua, "resizable", 0);
  props->high_dpi = lua_get_table_int(props->lua, "high_dpi", 0);
  props->fullscreen = lua_get_table_int(props->lua, "fullscreen", 0);

  lua_pop(props->lua, -1);

  return 1;
}

int _engine_sdl_init(TW_Window *props) {

  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);

  Uint32 flags = SDL_WINDOW_OPENGL;

  if (props->resizable) {
    flags |= SDL_WINDOW_RESIZABLE;
  }
  if (props->high_dpi) {
    flags |= SDL_WINDOW_ALLOW_HIGHDPI;
  }
  if (props->fullscreen) {
    flags |= SDL_WINDOW_FULLSCREEN;
  }

  props->sdl_window = SDL_CreateWindow("Dingus", SDL_WINDOWPOS_UNDEFINED,
                                       SDL_WINDOWPOS_UNDEFINED, props->width,
                                       props->height, flags);

  if (props->sdl_window == NULL) {
    app_log("Could not create TW_Window: %s\n", SDL_GetError());
    return 0;
  }

  return 1;
}

int _engine_opengl_init(TW_Window *props) {

  SDL_GLContext gl_context = SDL_GL_CreateContext(props->sdl_window);
  printf("OPENGL %s / GLSL %s\n", glGetString(GL_VERSION),
         glGetString(GL_SHADING_LANGUAGE_VERSION));

  glDepthFunc(GL_LESS);
  glFrontFace(GL_CW);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  SDL_GL_SetSwapInterval(props->vsync);

  app_log("OpenGL init.");

  // Stand in VAO, VBO requires this, so we provide one.
  glGenVertexArrays(1, &global_vao);
  glBindVertexArray(global_vao);

  engine_gl_check();

  return 1;
}

TW_Window *engine_init() {

  TW_Window *props = (TW_Window *)malloc(sizeof(TW_Window));

  if (!_engine_load_config(props) || !_engine_sdl_init(props) ||
      !_engine_opengl_init(props)) {
    free(props);
    return 0;
  }

  props->passed = 0;
  props->frame = 0;
  props->running = 1;
  props->kids = 0;

  app_log("Window created: %i %i", props->width, props->height);

  return props;
}