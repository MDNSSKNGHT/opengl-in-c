#include <GL/glew.h>
#include <SDL3_image/SDL_image.h>

#include "texture.h"

void texture_load(struct texture *texture, const char *filename) {
  SDL_Surface *surface = IMG_Load(filename);

  SDL_FlipSurface(surface, SDL_FLIP_VERTICAL);

  glGenTextures(1, &texture->gl_texture);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture->gl_texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB,
               GL_UNSIGNED_BYTE, surface->pixels);
  glGenerateMipmap(GL_TEXTURE_2D);
  /* unbind texture */
  glBindTexture(GL_TEXTURE_2D, 0);

  SDL_DestroySurface(surface);
}

void texture_delete(struct texture *texture) {
    glDeleteTextures(1, &texture->gl_texture);
}
