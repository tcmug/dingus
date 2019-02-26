#ifndef GFX_COMMON_HPP
#define GFX_COMMON_HPP

#define GL_GLEXT_PROTOTYPES

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>

#include <lua.h>

#include "component.h"
#include <config.h>

void _engine_gl_check(const char *file, const char *function, int line);

TW_Window *engine_init();
int engine_shutdown(TW_Window *props);

#define engine_gl_check() (_engine_gl_check(__FILE__, __FUNCTION__, __LINE__))

int lua_get_table_int(lua_State *L, const char *key, int def);
const char *lua_get_table_string(lua_State *L, const char *key,
                                 const char *def);

#endif
