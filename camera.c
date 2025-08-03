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

  center[0] = camera->position[0] + camera->orientation[0];
  center[1] = camera->position[1] + camera->orientation[1];
  center[2] = camera->position[2] + camera->orientation[2];

  glm_lookat(camera->position, center, camera->up, view);
  glm_perspective(glm_rad(camera->fov), (float)(int)(800 / 800),
                  camera->near_place, camera->far_plane, projection);
  glm_mat4_mul(projection, view, camera->matrix);
}
