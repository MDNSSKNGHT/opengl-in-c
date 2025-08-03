#include "camera.h"

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

void camera_process_input(struct camera *camera, SDL_Event *event) {
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
