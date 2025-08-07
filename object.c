#include <GL/glew.h>

#include "object.h"
#include "texture.h"

void object_build(struct object *object) {
  int current_index = 0;

  glGenVertexArrays(1, &object->vao);
  glBindVertexArray(object->vao);

  glGenBuffers(1, &object->ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, object->indices.size,
               object->indices.data, GL_STATIC_DRAW);

  glGenBuffers(1, &object->vbo);
  glBindBuffer(GL_ARRAY_BUFFER, object->vbo);
  glBufferData(GL_ARRAY_BUFFER, object->vertices.size, object->vertices.data,
               GL_STATIC_DRAW);

  /* position */
  glVertexAttribPointer(current_index, 3, GL_FLOAT, GL_FALSE,
                        sizeof(struct vertex_data),
                        (void *)offsetof(struct vertex_data, position));
  glEnableVertexAttribArray(current_index++);

  if (object->vertices.attributes & VERTEX_ATTRIBUTE_HAS_NORMAL) {
    /* normal */
    glVertexAttribPointer(current_index, 3, GL_FLOAT, GL_FALSE,
                          sizeof(struct vertex_data),
                          (void *)offsetof(struct vertex_data, normal));
    glEnableVertexAttribArray(current_index++);
  }

  if (object->vertices.attributes & VERTEX_ATTRIBUTE_HAS_COLOR) {
    /* color */
    glVertexAttribPointer(current_index, 3, GL_FLOAT, GL_FALSE,
                          sizeof(struct vertex_data),
                          (void *)offsetof(struct vertex_data, color));
    glEnableVertexAttribArray(current_index++);
  }

  if (object->vertices.attributes & VERTEX_ATTRIBUTE_HAS_TEXTURE) {
    /* texture_uv */
    glVertexAttribPointer(current_index, 2, GL_FLOAT, GL_FALSE,
                          sizeof(struct vertex_data),
                          (void *)offsetof(struct vertex_data, texture_uv));
    glEnableVertexAttribArray(current_index++);
  }

  /* bind to zero, avoid accidental modifications */
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void object_render(struct object *object) {
  /* TODO: handle error when texture does not exist */
  if (object->vertices.attributes & VERTEX_ATTRIBUTE_HAS_TEXTURE) {
    glBindTexture(GL_TEXTURE_2D, object->texture.gl_texture);
  }
  glBindVertexArray(object->vao);
  glDrawElements(GL_TRIANGLES, object->indices.count, GL_UNSIGNED_INT, 0);
}

void object_delete(struct object *object) {
  glDeleteVertexArrays(1, &object->vao);
  glDeleteBuffers(1, &object->vbo);
  glDeleteBuffers(1, &object->ebo);
  /* TODO: handle error when texture does not exist */
  if (object->vertices.attributes & VERTEX_ATTRIBUTE_HAS_TEXTURE) {
    texture_delete(&object->texture);
  }
}
