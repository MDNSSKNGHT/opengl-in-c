#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <GL/glew.h>
#include <cglm/cglm.h>

struct object {
  GLuint id;

  GLuint vao;
  GLuint ebo;
  GLuint vbo;

  /* TODO: vertices is just generic data */
  const GLfloat *vertices;
  unsigned vertices_size;

  const GLuint *indices;
  unsigned indices_count;
  unsigned indices_size;

  vec3 position;
  mat4 model;
};

void object_register(struct object *object);
void object_upload_mesh(struct object *object);
void object_register_attribute(struct object *object, int count,
                               int stride_count, int offset_count);
void object_compute_model(struct object *object);
void object_draw_mesh(struct object *object);
void object_unload_mesh(struct object *object);

#endif /* __OBJECT_H__ */
