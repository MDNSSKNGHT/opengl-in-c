#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aCol;
layout (location = 3) in vec2 aTex;

out vec3 color;
out vec2 textureCoordinates;
out vec3 normal;
out vec3 currentPosition;

uniform mat4 camera_matrix;
uniform mat4 model;

uniform mat4 light_color;

void main() {
  currentPosition = aPos;
  color = aCol;
  textureCoordinates = aTex;
  normal = aNormal;

  gl_Position = camera_matrix * vec4(aPos, 1.0);
}
