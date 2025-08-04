#include "object.h"

/* up to 10 objects can be rendered at the same time */
#define MAX_OBJECTS 10

static GLuint objects_current_index = 0;
static GLuint objects_attribute_indices[MAX_OBJECTS] = {0};

void object_register(struct object *object) {
  object->id = objects_current_index;
  objects_attribute_indices[object->id] = 0;
}

/*
 * generates the following:
 * vao
 * ebo
 * vbo
 */
void object_upload_mesh(struct object *object) {
  glGenVertexArrays(1, &object->vao);
  glBindVertexArray(object->vao);

  glGenBuffers(1, &object->ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, object->indices_size, object->indices,
               GL_STATIC_DRAW);

  glGenBuffers(1, &object->vbo);
  glBindBuffer(GL_ARRAY_BUFFER, object->vbo);
  glBufferData(GL_ARRAY_BUFFER, object->vertices_size, object->vertices,
               GL_STATIC_DRAW);

  /* bind to zero, avoid accidental modifications */
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void object_register_attribute(struct object *object, int count,
                               int stride_count, int offset_count) {
  GLuint index = objects_attribute_indices[object->id];

  glBindVertexArray(object->vao);
  glBindBuffer(GL_ARRAY_BUFFER, object->vbo);

  glVertexAttribPointer(index, count, GL_FLOAT, GL_FALSE,
                        stride_count * sizeof(GLfloat),
                        (void *)(offset_count * sizeof(GLfloat)));
  glEnableVertexAttribArray(index);

  /* for the next attribute */
  objects_attribute_indices[object->id]++;

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void object_draw_mesh(struct object *object) {
  glBindVertexArray(object->vao);
  glDrawElements(GL_TRIANGLES, object->indices_count, GL_UNSIGNED_INT, 0);
}

void object_unload_mesh(struct object *object) {
  glDeleteVertexArrays(1, &object->vao);
  glDeleteBuffers(1, &object->vbo);
  glDeleteBuffers(1, &object->ebo);
}
