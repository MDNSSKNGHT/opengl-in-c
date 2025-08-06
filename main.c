#include <GL/glew.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <cglm/cglm.h>
#include <stdio.h>
#include <stdlib.h>

#include "camera.h"
#include "object.h"
#include "shader.h"

#include "models/light.h"
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

  struct object pyramid_object, light_object;

  pyramid_object.vertices.data = PYRAMID_MODEL_VERTICES;
  pyramid_object.vertices.size = sizeof(PYRAMID_MODEL_VERTICES);
  pyramid_object.vertices.count = sizeof(PYRAMID_MODEL_VERTICES) / sizeof(PYRAMID_MODEL_VERTICES[0]);
  pyramid_object.vertices.attributes = VERTEX_ATTRIBUTE_HAS_NORMAL | VERTEX_ATTRIBUTE_HAS_COLOR | VERTEX_ATTRIBUTE_HAS_TEXTURE;

  pyramid_object.indices.data = PYRAMID_MODEL_INDICES;
  pyramid_object.indices.size = sizeof(PYRAMID_MODEL_INDICES);
  pyramid_object.indices.count = sizeof(PYRAMID_MODEL_INDICES) / sizeof(PYRAMID_MODEL_INDICES[0]);

  object_build(&pyramid_object);

  light_object.vertices.data = LIGHT_MODEL_VERTICES;
  light_object.vertices.size = sizeof(LIGHT_MODEL_VERTICES);
  light_object.vertices.count = sizeof(LIGHT_MODEL_VERTICES) / sizeof(LIGHT_MODEL_VERTICES[0]);
  /* only has position. */
  light_object.vertices.attributes = 0;

  light_object.indices.data = LIGHT_MODEL_INDICES;
  light_object.indices.size = sizeof(LIGHT_MODEL_INDICES);
  light_object.indices.count = sizeof(LIGHT_MODEL_INDICES) / sizeof(LIGHT_MODEL_INDICES[0]);

  object_build(&light_object);

  GLuint texture;
  glGenTextures(1, &texture);
  load_texture(texture, "textures/brick.jpg");

  struct shader light_shader;
  shader_from(&light_shader, "shaders/light.vert", "shaders/light.frag");

  mat4 light_model_mat = GLM_MAT4_IDENTITY_INIT;
  glm_translate(light_model_mat, (vec3){0.5f, 0.5f, 0.5f});

  vec4 light_color = {1.0f, 1.0f, 1.0f, 1.0f};

  /* TODO: abstract away shader uniforms */
  GLuint uniform_tex0_id = glGetUniformLocation(shader.program, "tex0");
  GLuint model = glGetUniformLocation(shader.program, "model");
  GLuint camera_matrix_id =
      glGetUniformLocation(shader.program, "camera_matrix");

  GLuint camera_position = glGetUniformLocation(shader.program, "camera_position");

  GLuint light_model = glGetUniformLocation(light_shader.program, "model");
  GLuint light_camera_matrix = glGetUniformLocation(light_shader.program, "camera_matrix");
  GLuint light_color_id = glGetUniformLocation(light_shader.program, "light_color");
  GLuint light_color_id_pyramid = glGetUniformLocation(shader.program, "light_color");
  GLuint light_position = glGetUniformLocation(shader.program, "light_position");

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
    glUniformMatrix4fv(model, 1, GL_FALSE, (float[3]){0.0f, 0.0f, 0.0f});

    glUniform1i(uniform_tex0_id, 0);
    glUniform4f(light_color_id_pyramid, light_color[0], light_color[1], light_color[2], light_color[3]);
    glUniform3f(light_position, 0.5f, 0.5f, 0.5f);
    glUniform3f(camera_position, camera.position[0], camera.position[1], camera.position[2]);

    glBindTexture(GL_TEXTURE_2D, texture);
    object_render(&pyramid_object);

    shader_use(&light_shader);
    glUniformMatrix4fv(light_camera_matrix, 1, GL_FALSE,
                       (const float *)camera.matrix);
    glUniformMatrix4fv(light_model, 1, GL_FALSE, (const float *)light_model_mat);
    glUniform4f(light_color_id, light_color[0], light_color[1], light_color[2], light_color[3]);

    object_render(&light_object);

    SDL_GL_SwapWindow(window);
  }

  object_delete(&pyramid_object);
  object_delete(&light_object);

  shader_delete(&shader);

  SDL_GL_DestroyContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
