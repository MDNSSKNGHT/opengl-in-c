#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <GL/glew.h>

#include "models/common.h"
#include "shader.h"
#include "texture.h"

#define VERTEX_ATTRIBUTE_HAS_NORMAL (1 << 0)
#define VERTEX_ATTRIBUTE_HAS_COLOR (1 << 1)
#define VERTEX_ATTRIBUTE_HAS_TEXTURE (1 << 2)

struct object {
  GLuint vao;
  GLuint ebo;
  GLuint vbo;

  struct {
    const struct vertex_data *data;
    unsigned size;
    unsigned count;
    unsigned char attributes;
  } vertices;

  struct {
    const unsigned int *data;
    unsigned size;
    unsigned count;
  } indices;

  struct texture texture;
  struct shader shader;
};

void object_build(struct object *object);
void object_render(struct object *object);
void object_delete(struct object *object);

#endif /* __OBJECT_H__ */
