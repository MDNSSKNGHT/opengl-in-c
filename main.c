#include <GL/glew.h>
#include <SDL3/SDL.h>
#include <cglm/cglm.h>
#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void load_texture(GLuint ref, const char *filepath) {
  int width, height, comp;
  unsigned char *buffer;

  stbi_set_flip_vertically_on_load(true);
  buffer = stbi_load(filepath, &width, &height, &comp, 0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, ref);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, buffer);
  glGenerateMipmap(GL_TEXTURE_2D);
  /* unbind texture */
  glBindTexture(GL_TEXTURE_2D, 0);

  stbi_image_free(buffer);
}

void compile_shader(GLuint ref, const char *filepath) {
  FILE *file;
  unsigned size;
  char *buffer, info[1024];
  GLint ret;

  file = fopen(filepath, "r");

  fseek(file, 0L, SEEK_END);
  size = ftell(file);
  fseek(file, 0L, SEEK_SET);

  buffer = malloc(size + 1);
  fread(buffer, 1, size, file);
  buffer[size] = '\0';
  fclose(file);

  glShaderSource(ref, 1, (const char **)&buffer, NULL);
  glCompileShader(ref);

  glGetShaderiv(ref, GL_COMPILE_STATUS, &ret);
  if (!ret) {
    glGetShaderInfoLog(ref, 1024, NULL, info);
    fprintf(stderr, "Shader compilation error: %s\n", info);
  }

  free(buffer);
}

int main() {
  /* coordinates (3 floats) - colors (3 floats) - texture (2 floats) */
  GLfloat vertices[] = {-0.5f, 0.0f, 0.5f,  0.83f, 0.70f, 0.44f, 0.0f, 0.0f,
                        -0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 0.0f,
                        0.5f,  0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f,
                        0.5f,  0.0f, 0.5f,  0.83f, 0.70f, 0.44f, 5.0f, 0.0f,
                        0.0f,  0.8f, 0.0f,  0.92f, 0.86f, 0.76f, 2.5f, 5.0f};
  GLuint indices[] = {0, 1, 2, 0, 2, 3, 0, 1, 4, 1, 2, 4, 2, 3, 4, 3, 0, 4};

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
    return 1;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  SDL_Window *window =
      SDL_CreateWindow("opengl-in-c", 800, 800, SDL_WINDOW_OPENGL);
  if (!window) {
    fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
    return 1;
  }

  SDL_GLContext gl_context = SDL_GL_CreateContext(window);
  if (!gl_context) {
    fprintf(stderr, "Failed to create OpenGL context: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  if (!SDL_GL_SetSwapInterval(1)) {
    fprintf(stderr, "Wait for vertical sync disabled: %s\n", SDL_GetError());
  }

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  glViewport(0, 0, 800, 800);

  GLuint vert = glCreateShader(GL_VERTEX_SHADER);
  compile_shader(vert, "shaders/default.vert");

  GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
  compile_shader(frag, "shaders/default.frag");

  GLuint shader_program = glCreateProgram();
  glAttachShader(shader_program, vert);
  glAttachShader(shader_program, frag);
  glLinkProgram(shader_program);

  glDeleteShader(vert);
  glDeleteShader(frag);

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                        (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                        (void *)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                        (void *)(6 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);

  /* bind to zero, avoid accidental modifications */
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  GLuint texture;
  glGenTextures(1, &texture);
  load_texture(texture, "textures/brick.jpg");

  GLuint uniform_scale_id = glGetUniformLocation(shader_program, "scale");
  GLuint uniform_tex0_id = glGetUniformLocation(shader_program, "tex0");

  GLuint model_id = glGetUniformLocation(shader_program, "model");
  GLuint view_id = glGetUniformLocation(shader_program, "view");
  GLuint projection_id = glGetUniformLocation(shader_program, "projection");

  float rotation = 0.0f;

  glEnable(GL_DEPTH_TEST);

  while (true) {
    SDL_Event event;
    SDL_PollEvent(&event);

    if (event.type == SDL_EVENT_QUIT) {
      break;
    }

    /* render loop */
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shader_program);

    mat4 model = GLM_MAT4_IDENTITY;
    mat4 view = GLM_MAT4_IDENTITY;
    mat4 projection = GLM_MAT4_IDENTITY;

    glm_spin(model, glm_rad(rotation), (vec3){0.0f, 1.0f, 0.0f});
    glm_translate(view, (vec3){0.0f, -0.5f, -2.0f});
    glm_perspective(glm_rad(45.0f), (float)(int)(800 / 800), 0.1f, 100.0f,
                    projection);
    rotation += 0.5f;

    glUniformMatrix4fv(model_id, 1, GL_FALSE, (const float *)model);
    glUniformMatrix4fv(view_id, 1, GL_FALSE, (const float *)view);
    glUniformMatrix4fv(projection_id, 1, GL_FALSE, (const float *)projection);

    glUniform1i(uniform_tex0_id, 0);
    glUniform1f(uniform_scale_id, 1.0f);

    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]),
                   GL_UNSIGNED_INT, 0);

    SDL_GL_SwapWindow(window);
  }

  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
  glDeleteProgram(shader_program);

  SDL_GL_DestroyContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
