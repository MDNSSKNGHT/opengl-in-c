#include <GL/glew.h>
#include <SDL3/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void compile_shader(GLuint ref, const char *filepath) {
  FILE *file;
  unsigned size;
  char *buffer;

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

  free(buffer);
}

int main() {
  /* coordinates - colors */
  GLfloat vertices[] = {
      -0.5f,        -0.5f * sqrtf(3.0f) / 3.0f,       0.0f, 0.8f, 0.3f,  0.02f,
      0.5f,         -0.5f * sqrtf(3.0f) / 3.0f,       0.0f, 0.8f, 0.3f,  0.02f,
      0.0f,         0.5f * sqrtf(3.0f) * 2.0f / 3.0f, 0.0f, 1.0f, 0.6f,  0.32f,
      -0.5f / 2.0f, 0.5f * sqrtf(3.0f) / 6.0f,        0.0f, 0.9f, 0.45f, 0.17f,
      0.5f / 2.0f,  0.5f * sqrtf(3.0f) / 6.0f,        0.0f, 0.9f, 0.45f, 0.17f,
      0.0f,         -0.5f * sqrtf(3.0f) / 3.0f,       0.0f, 0.8f, 0.3f,  0.02f,
  };
  GLuint indices[] = {0, 3, 5, 3, 2, 4, 5, 4, 1};

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

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                        (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                        (void *)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  /* bind to zero, avoid accidental modifications */
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  GLuint uniform_scale_id = glGetUniformLocation(shader_program, "scale");

  while (true) {
    SDL_Event event;
    SDL_PollEvent(&event);

    if (event.type == SDL_EVENT_QUIT) {
      break;
    }

    /* render loop */
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program);
    glUniform1f(uniform_scale_id, 0.5f);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

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
