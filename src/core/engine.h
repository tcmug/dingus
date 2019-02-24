#ifndef GFX_COMMON_HPP
#define GFX_COMMON_HPP

#define GL_GLEXT_PROTOTYPES

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>

#include "component.h"

void _engine_gl_check(const char *file, const char *function, int line);

TW_Window engine_init();
int engine_shutdown(TW_Window props);

#define engine_gl_check() (_engine_gl_check(__FILE__, __FUNCTION__, __LINE__))

#endif
