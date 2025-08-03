#include <GL/glew.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <cglm/cglm.h>
#include <stdio.h>
#include <stdlib.h>

#include "camera.h"
#include "object.h"
#include "shader.h"

#include "models/pyramid.h"

void load_texture(GLuint ref, const char *filename) {
  SDL_Surface *surface = IMG_Load(filename);

  SDL_FlipSurface(surface, SDL_FLIP_VERTICAL);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, ref);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB,
               GL_UNSIGNED_BYTE, surface->pixels);
  glGenerateMipmap(GL_TEXTURE_2D);
  /* unbind texture */
  glBindTexture(GL_TEXTURE_2D, 0);

  SDL_DestroySurface(surface);
}

int main() {
  struct object pyramid;
  OBJECT_FROM(pyramid, pyramid_vertices, pyramid_indices);

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

  SDL_SetWindowRelativeMouseMode(window, true);
  SDL_WarpMouseInWindow(window, 800 / 2.0f, 800 / 2.0f);

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

  struct shader shader;

  shader_from(&shader, "shaders/default.vert", "shaders/default.frag");

  object_register(&pyramid);
  object_create_mesh(&pyramid);

  /* vertices */
  object_link_attribute(&pyramid, 3, 8 * sizeof(GLfloat), (void *)0);
  /* colors */
  object_link_attribute(&pyramid, 3, 8 * sizeof(GLfloat),
                        (void *)(3 * sizeof(float)));
  /* texture coordinates */
  object_link_attribute(&pyramid, 2, 8 * sizeof(GLfloat),
                        (void *)(6 * sizeof(float)));

  GLuint texture;
  glGenTextures(1, &texture);
  load_texture(texture, "textures/brick.jpg");

  GLuint uniform_tex0_id = glGetUniformLocation(shader.program, "tex0");
  GLuint camera_matrix_id =
      glGetUniformLocation(shader.program, "camera_matrix");

  struct camera camera;

  camera_set_vectors(&camera, (vec3){0.0f, 0.0f, 2.0f},
                     (vec3){0.0f, 0.0f, -1.0f}, (vec3){0.0f, 1.0f, 0.0f});

  camera.fov = 45.0f;
  camera.near_place = 0.1f;
  camera.far_plane = 100.0f;

  camera.sensitivity = 10.0f;
  camera.speed = 0.05f;

  glEnable(GL_DEPTH_TEST);

  bool should_close = false;

  while (!should_close) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
    case SDL_EVENT_KEY_DOWN:
      camera_process_keyboard_input(&camera, &event);
      break;
    case SDL_EVENT_MOUSE_MOTION:
      camera_process_mouse_input(&camera, &event);
      break;
    case SDL_EVENT_QUIT:
      should_close = true;
      break;
    default:
      break;
    }

    /* render loop */
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader_use(&shader);

    camera_calculate_matrix(&camera);
    glUniformMatrix4fv(camera_matrix_id, 1, GL_FALSE,
                       (const float *)camera.matrix);

    glUniform1i(uniform_tex0_id, 0);

    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(pyramid.vao);
    glDrawElements(GL_TRIANGLES, pyramid.indices_count, GL_UNSIGNED_INT, 0);

    SDL_GL_SwapWindow(window);
  }

  object_delete_mesh(&pyramid);
  shader_delete(&shader);

  SDL_GL_DestroyContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
