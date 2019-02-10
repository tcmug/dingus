#ifndef TEXTURE_H
#define TEXTURE_H

#include "engine.h"

GLuint load_texture(const char *filename) {
  SDL_Surface *surface;
  GLuint texture = 0;

  surface = IMG_Load(filename);

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

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

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
    printf("got %s\n", filename);
  }
  return texture;
}

#endif