#ifndef __SHADER_H__
#define __SHADER_H__

#include <GL/glew.h>

struct shader {
  GLuint program;
};

void shader_from(struct shader *shader, const char *vert_source,
                 const char *frag_source);
void shader_use(struct shader *shader);
void shader_delete(struct shader *shader);
GLuint shader_get_uniform(struct shader *shader, const char *name);

#endif /* __SHADER_H__ */
