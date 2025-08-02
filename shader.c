#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>

#include "shader.h"

static void compile_shader(GLuint shader, const char *filename) {
  FILE *file;
  unsigned size;
  char *buffer, info[1024];
  GLint ret;

  file = fopen(filename, "r");

  fseek(file, 0L, SEEK_END);
  size = ftell(file);
  fseek(file, 0L, SEEK_SET);

  buffer = malloc(size + 1);
  fread(buffer, 1, size, file);
  buffer[size] = '\0';
  fclose(file);

  glShaderSource(shader, 1, (const char **)&buffer, NULL);
  glCompileShader(shader);

  glGetShaderiv(shader, GL_COMPILE_STATUS, &ret);
  if (!ret) {
    glGetShaderInfoLog(shader, 1024, NULL, info);
    fprintf(stderr, "Shader compilation error: %s\n", info);
  }

  free(buffer);
}

void shader_from(struct shader *shader, const char *vert_source,
                 const char *frag_source) {
  char info[1024];
  GLuint vert;
  GLuint frag;
  GLuint program;
  GLint ret;

  vert = glCreateShader(GL_VERTEX_SHADER);
  compile_shader(vert, vert_source);

  frag = glCreateShader(GL_FRAGMENT_SHADER);
  compile_shader(frag, frag_source);

  program = glCreateProgram();
  glAttachShader(program, vert);
  glAttachShader(program, frag);
  glLinkProgram(program);

  glDeleteShader(vert);
  glDeleteShader(frag);

  glGetProgramiv(program, GL_LINK_STATUS, &ret);
  if (!ret) {
    glGetProgramInfoLog(program, 1024, NULL, info);
    fprintf(stderr, "Shader program link error: %s\n", info);
    program = -1;
  }

  shader->program = program;
}

void shader_use(struct shader *shader) { glUseProgram(shader->program); }

void shader_delete(struct shader *shader) { glDeleteProgram(shader->program); }
