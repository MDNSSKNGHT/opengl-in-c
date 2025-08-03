#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <GL/glew.h>

struct object {
  GLuint id;

  GLuint vao;
  GLuint ebo;
  GLuint vbo;

  /* TODO: vertices is just generic data */
  GLfloat *vertices;
  unsigned vertices_size;
  unsigned vertices_count;

  GLuint *indices;
  unsigned indices_size;
  unsigned indices_count;
};

void object_register(struct object *object);
void object_create_mesh(struct object *object);
void object_link_attribute(struct object *object, GLint size, GLsizei stride,
                           const void *offset);
void object_delete_mesh(struct object *object);

#endif /* __OBJECT_H__ */
