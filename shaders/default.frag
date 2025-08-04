#version 330 core

in vec3 color;
in vec2 textureCoordinates;
in vec3 normal;
in vec3 currentPosition;

out vec4 FragColor;

uniform sampler2D tex0;

uniform vec4 light_color;
uniform vec3 light_position;

uniform vec3 camera_position;

void main() {
  float ambient = 0.2;

  vec3 normalu = normalize(normal);
  vec3 light_direction = normalize(light_position - currentPosition);

  float diffuse = max(dot(normalu, light_direction), 0.0);

  float specular_light = 0.5;
  vec3 view_direction = normalize(camera_position - currentPosition);
  vec3 reflection_direction = reflect(-light_direction, normalu);
  float specular_ammount = pow(max(dot(view_direction, reflection_direction), 0.0f), 8.0);
  float specular = specular_ammount * specular_light;

  FragColor = texture(tex0, textureCoordinates) * light_color * (diffuse + ambient + specular);
}
