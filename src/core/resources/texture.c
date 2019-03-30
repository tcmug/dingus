
#include "texture.h"
#include "../buffer.h"
#include "../log.h"

TW_Texture *TW_TextureLoad(const char *filename) {
  TW_Texture *t = (TW_Texture *)malloc(sizeof(TW_Texture));
  SDL_Surface *surface = IMG_Load(filename);

  memset(t, 0, sizeof(TW_Texture));

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

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // When MINifying the image, use a LINEAR blend of two mipmaps, each
    // filtered LINEARLY too
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    // Generate mipmaps, by the way.

    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT,
    // pixels);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, Mode,
                 GL_UNSIGNED_BYTE, surface->pixels);

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    engine_gl_check();

    SDL_FreeSurface(surface);
  }

  printf("Load: %s\n", filename);
  return t;
}

TW_Texture *TW_TextureRenderTarget(int w, int h, int hasdepth) {

  TW_Texture *f = (TW_Texture *)malloc(sizeof(TW_Texture));

  f->depth = 0;
  f->width = w;
  f->height = h;

  GLuint previous_framebuffer, previous_texture;
  glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint *)&previous_texture);
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint *)&previous_framebuffer);

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
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, f->width, f->height, 0, GL_RGB,
               GL_FLOAT, 0);

  // Attach 2D TW_Texture to this FBO
  glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                         GL_TEXTURE_2D, f->texture, 0);

  glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

  if (hasdepth == 1) {
    glGenRenderbuffers(1, &f->depth);
    glBindRenderbuffer(GL_RENDERBUFFER, f->depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, f->width,
                          f->height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, f->depth);

    // // Set "renderedTexture" as our colour attachement #0
    // glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
    // renderedTexture, 0);
  }
  // // Set the list of draw buffers.
  // GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
  // glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
  // */

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    app_log("Goddamn");
  }

  glBindFramebuffer(GL_FRAMEBUFFER, previous_framebuffer);
  glBindTexture(GL_TEXTURE_2D, previous_texture);
  //  glBindTexture(GL_TEXTURE_2D, 0);
  return f;
}

void TW_TextureFree(TW_Texture *t) {
  if (t->texture)
    glDeleteTextures(1, &t->texture);
  if (t->depth)
    glDeleteTextures(1, &t->depth);
  if (t->buffer)
    glDeleteFramebuffers(1, &t->buffer);
  free(t);
  app_log("Texture freed");
}

void TW_TextureStartRender(TW_Texture *t) {
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &t->previous_buffer);
  glGetIntegerv(GL_VIEWPORT, t->previous_viewport);
  glBindFramebuffer(GL_FRAMEBUFFER, t->buffer);
  glViewport(0, 0, t->width, t->height);
}

void TW_TextureEndRender(TW_Texture *t) {
  glBindFramebuffer(GL_FRAMEBUFFER, t->previous_buffer);
  glViewport(t->previous_viewport[0], t->previous_viewport[1],
             t->previous_viewport[2], t->previous_viewport[3]);
}

void TW_TextureDraw(TW_Texture *t, TW_Rectangle r) {
  // GL_STATIC_DRAW
  static TW_Vector3Buffer *va = 0;
  static TW_Vector2Buffer *pv = 0;

  if (!va) {
    va = TW_Vector3BufferCreate(4, GL_STREAM_DRAW);
    pv = TW_Vector2BufferCreate(4, GL_STREAM_DRAW);
  }

  va->data[0] = (TW_Vector3){r.x, r.y, 0};
  va->data[1] = (TW_Vector3){r.x, r.y + r.h, 0};
  va->data[2] = (TW_Vector3){r.x + r.w, r.y + r.h, 0};
  va->data[3] = (TW_Vector3){r.x + r.w, r.y, 0};

  pv->data[0] = (TW_Vector2){0, 0};
  pv->data[1] = (TW_Vector2){0, 1};
  pv->data[2] = (TW_Vector2){1, 1};
  pv->data[3] = (TW_Vector2){1, 0};

  TW_Vector3BufferUpdate(va, 4);
  TW_Vector2BufferUpdate(pv, 4);
  TW_Vector3BufferBind(va, 0);
  TW_Vector2BufferBind(pv, 1);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, t->texture);

  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  glBindTexture(GL_TEXTURE_2D, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}