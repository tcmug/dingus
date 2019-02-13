
#include "engine.h"
#include "log.h"

PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFrameBuffers;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2D;
PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
PFNGLBLITFRAMEBUFFERPROC glBlitFramebuffer;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
PFNGLUNIFORM1IPROC glUniform1i;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLDELETEPROGRAMPROC glDeleteProgram;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLDETACHSHADERPROC glDetachShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;

GLuint global_vao = 0;

#define GL_GET(var, type, name)                                                \
  var = (type)(SDL_GL_GetProcAddress(name));                                   \
  if (!var) {                                                                  \
    printf(" - %s\n", name);                                                   \
    return props;                                                              \
  }

window engine_init() {

  window props;

  props.width = 800;
  props.height = 600;

  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);

  props.window = SDL_CreateWindow(
      "Dingus", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, props.width,
      props.height,
      SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL);

  if (props.window == NULL) {
    printf("SDL_Init failed: %s\n", SDL_GetError());
    app_log("Could not create window...\n");
    return props;
  }

  SDL_GLContext gl_context = SDL_GL_CreateContext(props.window);
  printf("OPENGL %s / GLSL %s\n", glGetString(GL_VERSION),
         glGetString(GL_SHADING_LANGUAGE_VERSION));

  props.passed = 0;
  props.frame = 0;

  glDepthFunc(GL_LESS);

  glFrontFace(GL_CW);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#elif _MSC_VER
#elif __BORLANDC__
#elif __MINGW32__
#endif

  GL_GET(glGenerateMipmap, PFNGLGENERATEMIPMAPPROC, "glGenerateMipmap");
  GL_GET(glGenFramebuffers, PFNGLGENFRAMEBUFFERSPROC, "glGenFramebuffers");
  GL_GET(glDeleteFrameBuffers, PFNGLDELETEFRAMEBUFFERSPROC,
         "glDeleteFrameBuffers");

  GL_GET(glGetUniformLocation, PFNGLGETUNIFORMLOCATIONPROC,
         "glGetUniformLocation");
  GL_GET(glBindFramebuffer, PFNGLBINDFRAMEBUFFERPROC, "glBindFramebuffer");
  GL_GET(glFramebufferTexture, PFNGLFRAMEBUFFERTEXTUREPROC,
         "glFramebufferTexture");
  GL_GET(glCheckFramebufferStatus, PFNGLCHECKFRAMEBUFFERSTATUSPROC,
         "glCheckFramebufferStatus");
  GL_GET(glFramebufferTexture2D, PFNGLFRAMEBUFFERTEXTURE2DEXTPROC,
         "glFramebufferTexture2D");
  GL_GET(glGenRenderbuffers, PFNGLGENRENDERBUFFERSPROC, "glGenRenderbuffers");
  GL_GET(glBindRenderbuffer, PFNGLBINDRENDERBUFFERPROC, "glBindRenderbuffer");
  GL_GET(glRenderbufferStorage, PFNGLRENDERBUFFERSTORAGEPROC,
         "glRenderbufferStorage");
  GL_GET(glFramebufferRenderbuffer, PFNGLFRAMEBUFFERRENDERBUFFERPROC,
         "glFramebufferRenderbuffer");
  GL_GET(glBlitFramebuffer, PFNGLBLITFRAMEBUFFERPROC, "glBlitFramebuffer");
  GL_GET(glBindVertexArray, PFNGLBINDVERTEXARRAYPROC, "glBindVertexArray");
  GL_GET(glGenVertexArrays, PFNGLGENVERTEXARRAYSPROC, "glGenVertexArrays");
  GL_GET(glDeleteVertexArrays, PFNGLDELETEVERTEXARRAYSPROC,
         "glDeleteVertexArrays");
  GL_GET(glUniformMatrix4fv, PFNGLUNIFORMMATRIX4FVPROC, "glUniformMatrix4fv");
  GL_GET(glBindBuffer, PFNGLBINDBUFFERPROC, "glBindBuffer");
  GL_GET(glBindAttribLocation, PFNGLBINDATTRIBLOCATIONPROC,
         "glBindAttribLocation");
  GL_GET(glUniform1i, PFNGLUNIFORM1IPROC, "glUniform1i");
  GL_GET(glCreateProgram, PFNGLCREATEPROGRAMPROC, "glCreateProgram");
  GL_GET(glDeleteProgram, PFNGLDELETEPROGRAMPROC, "glDeleteProgram");
  GL_GET(glAttachShader, PFNGLATTACHSHADERPROC, "glAttachShader");
  GL_GET(glDetachShader, PFNGLATTACHSHADERPROC, "glDetachShader");
  GL_GET(glLinkProgram, PFNGLLINKPROGRAMPROC, "glLinkProgram");
  GL_GET(glGetProgramiv, PFNGLGETPROGRAMIVPROC, "glGetProgramiv");
  GL_GET(glGetProgramInfoLog, PFNGLGETPROGRAMINFOLOGPROC,
         "glGetProgramInfoLog");
  GL_GET(glCreateShader, PFNGLCREATESHADERPROC, "glCreateShader");
  GL_GET(glCompileShader, PFNGLCOMPILESHADERPROC, "glCompileShader");
  GL_GET(glShaderSource, PFNGLSHADERSOURCEPROC, "glShaderSource");
  GL_GET(glGetShaderiv, PFNGLGETSHADERIVPROC, "glGetShaderiv");
  GL_GET(glGetShaderInfoLog, PFNGLGETSHADERINFOLOGPROC, "glGetShaderInfoLog");
  GL_GET(glDeleteShader, PFNGLDELETESHADERPROC, "glDeleteShader");
  GL_GET(glUseProgram, PFNGLUSEPROGRAMPROC, "glUseProgram");
  GL_GET(glGenBuffers, PFNGLGENBUFFERSPROC, "glGenBuffers");
  GL_GET(glDeleteBuffers, PFNGLDELETEBUFFERSPROC, "glDeleteBuffers");
  GL_GET(glBufferData, PFNGLBUFFERDATAPROC, "glBufferData");
  GL_GET(glVertexAttribPointer, PFNGLVERTEXATTRIBPOINTERPROC,
         "glVertexAttribPointer");
  GL_GET(glEnableVertexAttribArray, PFNGLENABLEVERTEXATTRIBARRAYPROC,
         "glEnableVertexAttribArray");
  GL_GET(glVertexAttribPointer, PFNGLVERTEXATTRIBPOINTERPROC,
         "glVertexAttribPointer");
  GL_GET(glEnableVertexAttribArray, PFNGLENABLEVERTEXATTRIBARRAYPROC,
         "glEnableVertexAttribArray");
  GL_GET(glVertexAttribPointer, PFNGLVERTEXATTRIBPOINTERPROC,
         "glVertexAttribPointer");

#ifdef __GNUC__
#pragma GCC diagnostic pop
#elif _MSC_VER
#elif __BORLANDC__
#elif __MINGW32__
#endif

  SDL_GL_SetSwapInterval(0);

  // Stand in VAO, VBO requires this, so we provide one.
  glGenVertexArrays(1, &global_vao);
  glBindVertexArray(global_vao);

  engine_gl_check();

  app_log("Window created: %i %i", props.width, props.height);

  return props;
}

int engine_shutdown(window props) {
  glDeleteVertexArrays(1, &global_vao);
  SDL_DestroyWindow(props.window);
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
