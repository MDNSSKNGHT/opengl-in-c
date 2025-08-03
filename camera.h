#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <cglm/cglm.h>

struct camera {
  vec3 position;
  vec3 orientation;
  vec3 up;

  float fov;
  float near_place;
  float far_plane;

  mat4 matrix;
};

void camera_set_vectors(struct camera *camera, vec3 position, vec3 orientation,
                        vec3 up);
void camera_calculate_matrix(struct camera *camera);

#endif /* __CAMERA_H__ */
