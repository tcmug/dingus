#ifndef TW_CORE_ENGINE_H
#define TW_CORE_ENGINE_H

#include "lua.h"
#include "resources.h"
#include "sdl.h"
#include "ui.h"
#include <config.h>

TW_Window *engine_init();
int engine_shutdown(TW_Window *props);
void engine_set_cursor_mode(TW_Window *props, TW_CursorMode mode);
void engine_report_cursor_position(TW_Window *props, const SDL_Event *ev);

#endif
