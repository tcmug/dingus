#ifndef TW_CORE_ENGINE_H
#define TW_CORE_ENGINE_H

#include "sdl.h"

#include <lua.h>

#include "ui.h"
#include <config.h>

TW_Window *engine_init();
int engine_shutdown(TW_Window *props);
void engine_set_cursor_mode(TW_Window *props, TW_CursorMode mode);
void engine_report_cursor_position(TW_Window *props, const SDL_Event *ev);

int lua_get_table_int(lua_State *L, const char *key, int def);
const char *lua_get_table_string(lua_State *L, const char *key,
                                 const char *def);
void *lua_get_table_userdata(lua_State *L, const char *key, void *def);

void l_prep_kids(lua_State *L, TW_Component *comp);

#endif
