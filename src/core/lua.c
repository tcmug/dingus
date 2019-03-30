
#include "lua.h"
#include "../fun/object.h"
#include "ui.h"

int callback_index = -1;

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

void *lua_get_table_userdata(lua_State *L, const char *key, void *def) {
  void *value = 0;
  lua_pushstring(L, key);
  lua_gettable(L, -2);
  if (lua_islightuserdata(L, -1)) {
    value = lua_touserdata(L, -1);
  } else {
    value = def;
  }
  lua_pop(L, 1);
  return value;
}

void l_ENTITY_PROCESS_CHILDREN(lua_State *L, TW_Entity *comp) {
  comp->kids = 0;
  lua_pushstring(L, "children");
  lua_gettable(L, -2);
  if (lua_istable(L, -1)) {
    lua_pushnil(L);
    while (lua_next(L, -2) != 0) {
      if (lua_islightuserdata(L, -1)) {
        TW_EntityAppendChild(comp, (TW_Entity *)lua_touserdata(L, -1));
      }
      lua_pop(L, 1);
    }
  }
  lua_pop(L, 1);
}

static int l_VIEW(lua_State *L) {
  int x = lua_get_table_int(L, "x", 0);
  int y = lua_get_table_int(L, "y", 0);
  int w = lua_get_table_int(L, "w", 50);
  int h = lua_get_table_int(L, "h", 50);
  int depth = lua_get_table_int(L, "depth", 0);
  app_log("VIEW(%i,%i %i,%i %u)", x, y, w, h, depth);
  TW_Entity *comp =
      VIEW(.color = {0, 0, 0, 0}, .rect = {x, y, w, h}, .hasDepth = depth);
  l_ENTITY_PROCESS_CHILDREN(L, comp);
  lua_pushlightuserdata(L, comp);
  return 1;
}

static int l_parent(lua_State *L) {
  TW_Entity *comp = (TW_Entity *)lua_touserdata(L, -1);
  lua_pushlightuserdata(L, comp->parent);
  return 1;
}

static int l_insert(lua_State *L) {
  if (lua_isuserdata(L, -2)) {
    TW_Entity *comp = lua_touserdata(L, -2);
    TW_EntityAppendChild(comp, (TW_Entity *)lua_touserdata(L, -1));
    TW_EntityRerender(comp);
    lua_pushlightuserdata(L, comp->parent);
  } else {
    app_log("not user data");
  }
  return 0;
}

static int l_ENTITY(lua_State *L) {

  int x = lua_get_table_int(L, "x", 0);
  int y = lua_get_table_int(L, "y", 0);
  int w = lua_get_table_int(L, "w", 50);
  int h = lua_get_table_int(L, "h", 50);

  app_log("ENTITY(%i,%i %i,%i)", x, y, w, h);
  TW_Entity *comp = ENTITY(.rect = {x, y, w, h});

  l_ENTITY_PROCESS_CHILDREN(L, comp);
  lua_pushlightuserdata(L, comp);
  return 1;
}

static int l_quit(lua_State *L) {
  SDL_Quit();
  return 0;
}

static int l_text(lua_State *L) {
  int x = lua_get_table_int(L, "x", 0);
  int y = lua_get_table_int(L, "y", 0);
  int w = lua_get_table_int(L, "w", 50);
  int h = lua_get_table_int(L, "h", 50);

  app_log("TEXT(%i,%i %i,%i)", x, y, w, h);

  int lua_click = 0;
  lua_pushstring(L, "click");
  lua_gettable(L, -2);
  if (lua_isfunction(L, -1)) {
    if (callback_index == -1) {
      lua_newtable(L); // create table for functions
      callback_index = luaL_ref(L, LUA_REGISTRYINDEX);
    }
    lua_rawgeti(L, LUA_REGISTRYINDEX, callback_index);
    lua_pushvalue(L, -2);
    lua_click = luaL_ref(L, -2);
    lua_pop(L, 1);
  }
  lua_pop(L, 1);

  const char *cstr = lua_get_table_string(L, "text", "null");

  TW_Entity *comp = TEXT(.rect = {x, y, w, h},
                         .color =
                             {
                                 1,
                                 1,
                                 1,
                             },
                         .text = cstr, .lua_click = lua_click);

  l_ENTITY_PROCESS_CHILDREN(L, comp);
  lua_pushlightuserdata(L, comp);
  return 1;
}

void lua_get_callback_method(lua_State *L, int index) {
  lua_rawgeti(L, LUA_REGISTRYINDEX, callback_index);
  lua_rawgeti(L, -1, index);
}

void TW_RegisterLua(lua_State *L) {
  lua_pushcfunction(L, l_VIEW);
  lua_setglobal(L, "VIEW");

  lua_pushcfunction(L, l_text);
  lua_setglobal(L, "TEXT");

  lua_pushcfunction(L, l_insert);
  lua_setglobal(L, "insert");

  lua_pushcfunction(L, l_parent);
  lua_setglobal(L, "parent");

  lua_pushcfunction(L, l_quit);
  lua_setglobal(L, "quit");

  lua_pushcfunction(L, l_ENTITY);
  lua_setglobal(L, "ENTITY");

  lua_pushcfunction(L, l_SCENE);
  lua_setglobal(L, "SCENE");

  lua_pushcfunction(L, l_SHADER);
  lua_setglobal(L, "SHADER");

  lua_pushcfunction(L, l_MODEL);
  lua_setglobal(L, "MODEL");

  lua_pushcfunction(L, l_OBJECT);
  lua_setglobal(L, "OBJECT");
}