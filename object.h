#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <GL/glew.h>
#include <cglm/cglm.h>

#define OBJECT_SET_DATA(o, v, i)                                               \
  do {                                                                         \
    o.vertices = v;                                                            \
    o.vertices_size = sizeof(v);                                               \
    o.indices = i;                                                             \
    o.indices_size = sizeof(i);                                                \
    o.indices_count = sizeof(i) / sizeof(i[0]);                                \
  } while (0)

#define OBJECT_SET_POSITION(o, p)                                              \
  do {                                                                         \
    glm_vec3_copy(p, o.position);                                              \
    glm_mat4_copy((mat4)GLM_MAT4_IDENTITY_INIT, o.model);                      \
    glm_translate(o.model, o.position);                                        \
  } while (0)

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
void object_draw_mesh(struct object *object);
void object_unload_mesh(struct object *object);

#endif /* __OBJECT_H__ */
