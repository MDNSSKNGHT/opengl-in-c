#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 camera_matrix;
uniform mat4 model;

void main() {
    gl_Position = camera_matrix * model * vec4(aPos, 1.0f);
}
