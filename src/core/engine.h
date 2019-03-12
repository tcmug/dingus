#ifndef TW_CORE_ENGINE_H
#define TW_CORE_ENGINE_H

#include "sdl.h"

#include <lua.h>

#include "ui.h"
#include <config.h>

TW_Window *engine_init();
int engine_shutdown(TW_Window *props);

int lua_get_table_int(lua_State *L, const char *key, int def);
const char *lua_get_table_string(lua_State *L, const char *key,
                                 const char *def);

#endif
