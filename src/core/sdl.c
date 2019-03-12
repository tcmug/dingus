
#include "sdl.h"

void _engine_gl_check(const char *file, const char *function, int line) {

  GLenum err;

  while ((err = glGetError()) != GL_NO_ERROR) {
    printf("%s: %u => %s (%s)\n", file, line, function);
    //, gluErrorString(err));
    exit(1);
  }
}