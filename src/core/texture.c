
#include "texture.h"
#include "buffer.h"
#include "log.h"

texture *texture_load(const char *filename) {
  texture *t = (texture *)malloc(sizeof(texture));
  SDL_Surface *surface = IMG_Load(filename);

  memset(t, 0, sizeof(texture));

  if (surface) {

    int Mode = GL_RGBA;
    int Colors = surface->format->BytesPerPixel;

    if (Colors == 4) {
      if (surface->format->Rmask == 0x000000ff)
        Mode = GL_RGBA;
      else
        Mode = GL_BGRA;
    } else if (Colors == 3) {
      if (surface->format->Rmask == 0x000000ff)
        Mode = GL_RGB;
      else
        Mode = GL_BGR;
    } else {
      Mode = GL_COLOR_INDEX;
    }

    glGenTextures(1, &t->texture);
    glBindTexture(GL_TEXTURE_2D, t->texture);

    t->width = surface->w;
    t->height = surface->h;

    float pixels[] = {1.0f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
                      1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.5f};

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT,
    // pixels);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, Mode,
                 GL_UNSIGNED_BYTE, surface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    engine_gl_check();

    SDL_FreeSurface(surface);
  }
  return t;
}

texture *texture_render_target(int w, int h) {

  texture *f = (texture *)malloc(sizeof(texture));

  f->width = w;
  f->height = h;

  glGenFramebuffers(1, &f->buffer);
  glBindFramebuffer(GL_FRAMEBUFFER, f->buffer);

  glGenTextures(1, &f->texture);
  glBindTexture(GL_TEXTURE_2D, f->texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, f->width, f->height, 0, GL_RGB,
               GL_FLOAT, 0);

  // Attach 2D texture to this FBO
  glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                         GL_TEXTURE_2D, f->texture, 0);

  glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

  /*
  GLuint depthrenderbuffer;
  glGenRenderbuffers(1, &depthrenderbuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
  GL_RENDERBUFFER, depthrenderbuffer);

  // Set "renderedTexture" as our colour attachement #0
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture,
  0);

  // Set the list of draw buffers.
  GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
  */

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    app_log("Goddamn");
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
  return f;
}

void texture_destroy(texture *t) {
  glDeleteTextures(1, &t->texture);
  if (t->buffer)
    glDeleteFrameBuffers(1, &t->buffer);
  free(t);
}

void texture_start_render(texture *t) {
  glBindFramebuffer(GL_FRAMEBUFFER, t->buffer);
  glViewport(0, 0, t->width, t->height);
}

void texture_end_render(texture *t) { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void texture_draw(texture *t, rectangle r) {

  // GL_STATIC_DRAW
  static vector_buffer *va = 0;
  static point_buffer *pv = 0;

  if (!va) {
    va = vector_buffer_create(4, GL_STREAM_DRAW);
    pv = point_buffer_create(4, GL_STREAM_DRAW);
  }

  va->data[0] = (vector){r.x, r.y, 0};
  va->data[1] = (vector){r.x, r.y + r.h, 0};
  va->data[2] = (vector){r.x + r.w, r.y + r.h, 0};
  va->data[3] = (vector){r.x + r.w, r.y, 0};

  pv->data[0] = (point){0, 0};
  pv->data[1] = (point){0, 1};
  pv->data[2] = (point){1, 1};
  pv->data[3] = (point){1, 0};

  vector_buffer_update(va, 4);
  point_buffer_update(pv, 4);
  vector_buffer_bind(va, 0);
  point_buffer_bind(pv, 1);

  glBindTexture(GL_TEXTURE_2D, t->texture);

  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  glBindTexture(GL_TEXTURE_2D, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}