#ifndef TW_LUA_H
#define TW_LUA_H

#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

#include "ui.h"

int lua_get_table_int(lua_State *L, const char *key, int def);
const char *lua_get_table_string(lua_State *L, const char *key,
                                 const char *def);
void *lua_get_table_userdata(lua_State *L, const char *key, void *def);
void lua_get_callback_method(lua_State *L, int index);
void l_ENTITY_PROCESS_CHILDREN(lua_State *L, TW_Entity *comp);
void TW_RegisterLua(lua_State *L);

#endif