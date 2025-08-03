#include <math.h>

#include "camera.h"

#define UNUSED(e) ((void)e)

void camera_set_vectors(struct camera *camera, vec3 position, vec3 orientation,
                        vec3 up) {
  glm_vec3_copy(position, camera->position);
  glm_vec3_copy(orientation, camera->orientation);
  glm_vec3_copy(up, camera->up);
}

void camera_calculate_matrix(struct camera *camera) {
  mat4 view = GLM_MAT4_IDENTITY_INIT;
  mat4 projection = GLM_MAT4_IDENTITY_INIT;
  vec3 center;

  glm_vec3_add(camera->position, camera->orientation, center);
  glm_lookat(camera->position, center, camera->up, view);
  glm_perspective(glm_rad(camera->fov), (float)(int)(800 / 800),
                  camera->near_place, camera->far_plane, projection);
  glm_mat4_mul(projection, view, camera->matrix);
}

void camera_process_keyboard_input(struct camera *camera, SDL_Event *event) {
  vec3 vec;

  switch (event->key.scancode) {
  case SDL_SCANCODE_W:
    glm_vec3_scale(camera->orientation, camera->speed, vec);
    glm_vec3_add(camera->position, vec, camera->position);
    break;
  case SDL_SCANCODE_A:
    glm_vec3_crossn(camera->orientation, camera->up, vec);
    glm_vec3_negate(vec);
    glm_vec3_scale(vec, camera->speed, vec);
    glm_vec3_add(camera->position, vec, camera->position);
    break;
  case SDL_SCANCODE_S:
    glm_vec3_negate_to(camera->orientation, vec);
    glm_vec3_scale(vec, camera->speed, vec);
    glm_vec3_add(camera->position, vec, camera->position);
    break;
  case SDL_SCANCODE_D:
    glm_vec3_crossn(camera->orientation, camera->up, vec);
    glm_vec3_scale(vec, camera->speed, vec);
    glm_vec3_add(camera->position, vec, camera->position);
    break;
  default:
    break;
  }
}

void camera_process_mouse_input(struct camera *camera, SDL_Event *event) {
  float x, y, rotation_x, rotation_y;
  vec3 orientation, orientation_normalized, axis;

  UNUSED(event);

  SDL_GetMouseState(&x, &y);

  /* TODO: pass width and height of window */
  rotation_x = camera->sensitivity * (y - (800 / 2.0f)) / 800;
  rotation_y = camera->sensitivity * (x - (800 / 2.0f)) / 800;

  glm_vec3_copy(camera->orientation, orientation);
  glm_vec3_crossn(orientation, camera->up, axis);
  glm_vec3_rotate(orientation, glm_rad(-rotation_x), axis);

  glm_vec3_normalize_to(orientation, orientation_normalized);

  /* TODO: find a better way to prevent barrel roll */
  if (fabsf(glm_vec3_angle(orientation, camera->up) - glm_rad(90.0f)) <=
      glm_rad(85.0f)) {
    glm_vec3_copy(orientation, camera->orientation);
  }

  glm_vec3_rotate(orientation, glm_rad(-rotation_y), camera->up);
  glm_vec3_copy(orientation, camera->orientation);

  SDL_WarpMouseInWindow(NULL, 800 / 2.0f, 800 / 2.0f);
}
