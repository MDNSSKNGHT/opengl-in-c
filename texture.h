#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "OpenGL/gltypes.h"

struct texture {
    GLuint gl_texture;
};

void texture_load(struct texture *texture, const char *filename);
void texture_delete(struct texture *texture);

#endif /* __TEXTURE_H__ */
