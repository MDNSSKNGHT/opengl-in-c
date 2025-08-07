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
#include "texture.h"

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

  struct object pyramid_object, light_object;

  pyramid_object.vertices.data = PYRAMID_MODEL_VERTICES;
  pyramid_object.vertices.size = sizeof(PYRAMID_MODEL_VERTICES);
  pyramid_object.vertices.count =
      sizeof(PYRAMID_MODEL_VERTICES) / sizeof(PYRAMID_MODEL_VERTICES[0]);
  pyramid_object.vertices.attributes = VERTEX_ATTRIBUTE_HAS_NORMAL |
                                       VERTEX_ATTRIBUTE_HAS_COLOR |
                                       VERTEX_ATTRIBUTE_HAS_TEXTURE;

  pyramid_object.indices.data = PYRAMID_MODEL_INDICES;
  pyramid_object.indices.size = sizeof(PYRAMID_MODEL_INDICES);
  pyramid_object.indices.count =
      sizeof(PYRAMID_MODEL_INDICES) / sizeof(PYRAMID_MODEL_INDICES[0]);

  shader_from(&pyramid_object.shader, "shaders/default.vert", "shaders/default.frag");
  texture_load(&pyramid_object.texture, "textures/brick.jpg");
  object_build(&pyramid_object);

  light_object.vertices.data = LIGHT_MODEL_VERTICES;
  light_object.vertices.size = sizeof(LIGHT_MODEL_VERTICES);
  light_object.vertices.count =
      sizeof(LIGHT_MODEL_VERTICES) / sizeof(LIGHT_MODEL_VERTICES[0]);
  /* only has position. */
  light_object.vertices.attributes = 0;

  light_object.indices.data = LIGHT_MODEL_INDICES;
  light_object.indices.size = sizeof(LIGHT_MODEL_INDICES);
  light_object.indices.count =
      sizeof(LIGHT_MODEL_INDICES) / sizeof(LIGHT_MODEL_INDICES[0]);

  shader_from(&light_object.shader, "shaders/light.vert", "shaders/light.frag");
  object_build(&light_object);

  mat4 light_model_mat = GLM_MAT4_IDENTITY_INIT;
  glm_translate(light_model_mat, (vec3){0.5f, 0.5f, 0.5f});

  vec4 light_color = {1.0f, 1.0f, 1.0f, 1.0f};

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

    shader_use(&pyramid_object.shader);
    /* TODO: abstract away shader uniforms */
    glGetUniformLocation(pyramid_object.shader.program, "camera_matrix");
    camera_calculate_matrix(&camera);
    glUniformMatrix4fv(shader_get_uniform(&pyramid_object.shader, "camera_matrix"), 1,
                       GL_FALSE, (const float *)camera.matrix);
    glUniformMatrix4fv(shader_get_uniform(&pyramid_object.shader, "model"), 1, GL_FALSE,
                       (float[3]){0.0f, 0.0f, 0.0f});

    glUniform1i(shader_get_uniform(&pyramid_object.shader, "tex0"), 0);
    glUniform4f(shader_get_uniform(&pyramid_object.shader, "light_color"), light_color[0],
                light_color[1], light_color[2], light_color[3]);
    glUniform3f(shader_get_uniform(&pyramid_object.shader, "light_position"), 0.5f, 0.5f,
                0.5f);
    glUniform3f(shader_get_uniform(&pyramid_object.shader, "camera_position"),
                camera.position[0], camera.position[1], camera.position[2]);

    object_render(&pyramid_object);

    shader_use(&light_object.shader);
    /* TODO: abstract away shader uniforms */
    glUniformMatrix4fv(shader_get_uniform(&light_object.shader, "camera_matrix"), 1,
                       GL_FALSE, (const float *)camera.matrix);
    glUniformMatrix4fv(shader_get_uniform(&light_object.shader, "model"), 1, GL_FALSE,
                       (const float *)light_model_mat);
    glUniform4f(shader_get_uniform(&light_object.shader, "light_color"),
                light_color[0], light_color[1], light_color[2], light_color[3]);

    object_render(&light_object);

    SDL_GL_SwapWindow(window);
  }

  object_delete(&pyramid_object);
  object_delete(&light_object);

  SDL_GL_DestroyContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
