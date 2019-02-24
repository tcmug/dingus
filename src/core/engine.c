
#include "engine.h"
#include "log.h"

GLuint global_vao = 0;

#define GL_GET(var, type, name)                                                \
  var = (type)(SDL_GL_GetProcAddress(name));                                   \
  if (!var) {                                                                  \
    printf(" - %s\n", name);                                                   \
    return props;                                                              \
  }

TW_Window engine_init() {

  TW_Window props;

  props.width = 800;
  props.height = 600;

  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);

  props.TW_Window = SDL_CreateWindow(
      "Dingus", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, props.width,
      props.height,
      SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL);

  if (props.TW_Window == NULL) {
    printf("SDL_Init failed: %s\n", SDL_GetError());
    app_log("Could not create TW_Window...\n");
    return props;
  }

  SDL_GLContext gl_context = SDL_GL_CreateContext(props.TW_Window);
  printf("OPENGL %s / GLSL %s\n", glGetString(GL_VERSION),
         glGetString(GL_SHADING_LANGUAGE_VERSION));

  props.passed = 0;
  props.frame = 0;

  glDepthFunc(GL_LESS);

  glFrontFace(GL_CW);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  SDL_GL_SetSwapInterval(0);

  app_log("OpenGL init.");

  // Stand in VAO, VBO requires this, so we provide one.
  glGenVertexArrays(1, &global_vao);
  glBindVertexArray(global_vao);

  engine_gl_check();

  app_log("TW_Window created: %i %i", props.width, props.height);

  return props;
}

int engine_shutdown(TW_Window props) {
  glDeleteVertexArrays(1, &global_vao);
  SDL_DestroyWindow(props.TW_Window);
  SDL_Quit();
  return 0;
}

// #include <GL/glu.h>

void _engine_gl_check(const char *file, const char *function, int line) {
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR) {
    printf("%s: %u => %s (%s)\n", file, line, function);
    //, gluErrorString(err));
    exit(1);
  }
}
