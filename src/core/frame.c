
#include "frame.h"

#include "log.h"

frame *frame_create(int w, int h) {

  frame *f = (frame *)malloc(sizeof(frame));

  f->width = w;
  f->height = h;

  glGenFramebuffers(1, &f->buffer);
  glBindFramebuffer(GL_FRAMEBUFFER, f->buffer);

  glGenTextures(1, &f->TW_Texture);
  glBindTexture(GL_TEXTURE_2D, f->TW_Texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, f->width, f->height, 0, GL_RGB,
               GL_FLOAT, 0);

  // Attach 2D TW_Texture to this FBO
  glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                         GL_TEXTURE_2D, f->TW_Texture, 0);

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

void frame_destroy(frame *f) {
  glDeleteFrameBuffers(1, &f->buffer);
  glDeleteTextures(1, &f->TW_Texture);
}

void frame_bind(frame *f) {
  glBindFramebuffer(GL_FRAMEBUFFER, f->buffer);
  glViewport(0, 0, f->width, f->height);
}

void frame_unbind(frame *f) { glBindFramebuffer(GL_FRAMEBUFFER, 0); }